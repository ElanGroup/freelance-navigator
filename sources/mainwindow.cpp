#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QScrollBar>
#include <QDesktopServices>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settings.h"
#include "Upwork/upworkapiclient.h"
#include "Upwork/upworkcategory.h"
#include "upworkjobsearcher.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Upwork;
using namespace FreelanceNavigator::Widgets;

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(new Settings()),
    m_upworkApiClient(new UpworkApiClient(m_settings, this))
{
    ui->setupUi(this);
    setWindowState(windowState() | Qt::WindowMaximized);

    createTrayIcon();
    setupConnections();
    setupUpworkFilters();

    ui->statusBar->showMessage(tr("Connect to Upwork service..."));
    m_upworkApiClient->initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_settings;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    hide();
    m_trayIcon->show();
    event->ignore();
}

void MainWindow::createTrayIcon()
{
    m_actionRestore = new QAction(tr("Open Freelance Navigator"), this);

    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_actionRestore);
    m_trayIconMenu->addAction(ui->actionExit);

    m_trayIcon = new QSystemTrayIcon(QIcon(":/Resources/FreelanceNavigator.ico"), this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setToolTip(tr("Freelance Navigator"));
}

void MainWindow::setupConnections()
{
    connect(ui->actionExit, &QAction::triggered, qApp, &QCoreApplication::quit);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->actionUpworkLogIn, &QAction::triggered,
            m_upworkApiClient, &UpworkApiClient::initialize);
    connect(ui->actionUpworkLogOut, &QAction::triggered, this, &MainWindow::logOutFromUpwork);
    connect(ui->upworkJobTypeComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::upworkJobTypeChanged);
    connect(ui->upworkSearchButton, &QPushButton::clicked, this, &MainWindow::searchUpworkJobs);
    connect(m_upworkApiClient, &UpworkApiClient::error, this, &MainWindow::processUpworkError);
    connect(m_upworkApiClient, &UpworkApiClient::warning, this, &MainWindow::processUpworkWarning);
    connect(m_upworkApiClient, &UpworkApiClient::initialized,
            this, &MainWindow::processUpworkInitialization);
    connect(m_upworkApiClient, &UpworkApiClient::categoriesLoaded,
            this, &MainWindow::fillUpworkCategories);
    connect(ui->upworkJobListWidget, &JobListWidget::jobOpenned, this, &MainWindow::openUpworkJob);
    connect(m_actionRestore, &QAction::triggered, this, &MainWindow::restoreWindow);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
}

void MainWindow::setupUpworkFilters()
{
    ui->upworkPostedDateComboBox->setCurrentIndex(m_settings->upworkPostedDateRange());

    ui->upworkBudgetLabel->setVisible(false);
    ui->upworkMinBudgetLabel->setVisible(false);
    ui->upworkMinBudgetLineEdit->setVisible(false);
    ui->upworkMaxBudgetLabel->setVisible(false);
    ui->upworkMaxBudgetLineEdit->setVisible(false);
    ui->upworkJobTypeComboBox->setCurrentIndex(m_settings->upworkJobType());

    ui->upworkMinBudgetLineEdit->setValidator(new QIntValidator(0, 9999999, this));
    ui->upworkMaxBudgetLineEdit->setValidator(new QIntValidator(0, 9999999, this));
    if (m_settings->upworkMinBudget() != -1)
    {
        ui->upworkMinBudgetLineEdit->setText(QString::number(m_settings->upworkMinBudget()));
    }
    if (m_settings->upworkMaxBudget() != -1)
    {
        ui->upworkMaxBudgetLineEdit->setText(QString::number(m_settings->upworkMaxBudget()));
    }
}

void MainWindow::showAbout()
{
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::upworkJobTypeChanged(int index)
{
    JobType jobType = static_cast<JobType>(index);
    ui->upworkBudgetLabel->setVisible(jobType == JobType::Fixed);
    ui->upworkMinBudgetLabel->setVisible(jobType == JobType::Fixed);
    ui->upworkMaxBudgetLabel->setVisible(jobType == JobType::Fixed);
    ui->upworkMinBudgetLineEdit->setVisible(jobType == JobType::Fixed);
    ui->upworkMaxBudgetLineEdit->setVisible(jobType == JobType::Fixed);
}

void MainWindow::processUpworkError(UpworkApiError error)
{
    ui->statusBar->clearMessage();
    updateUpworkSearchButtonState();

    QString message;
    switch (error)
    {
    case UpworkApiError::ConnectionError:
        message = tr("Connection error. Please check your internet connection.");
        break;
    case UpworkApiError::AuthenticationError:
        logOutFromUpwork();
        message = tr("Upwork authentication error. Could you please log in to Upwork service again?");
        break;
    case UpworkApiError::ServiceError:
        message = tr("Upwork service is unavailable. Please try again later.");
        break;
    }
    QMessageBox::critical(this, tr("Error"), message);
}

void MainWindow::processUpworkWarning(UpworkApiWarning warning)
{
    ui->statusBar->clearMessage();

    QString message;
    switch (warning)
    {
    case UpworkApiWarning::AuthorizationRequired:
        message = tr("Upwork authorization was not finished. You should login to Upwork service.");
        break;
    }
    QMessageBox::warning(this, tr("Warning"), message);
}

void MainWindow::processUpworkInitialization()
{
    updateUpworkActions(true);
    updateUpworkSearchButtonState();
    if (ui->upworkCategoryComboBox->count() == 0)
    {
        ui->statusBar->showMessage(tr("Load categories..."));
        m_upworkApiClient->loadCategories();
    }
}

void MainWindow::fillUpworkCategories(const QList<QSharedPointer<UpworkCategory>> & categories)
{
    ui->statusBar->clearMessage();

    foreach (const QSharedPointer<UpworkCategory> & category, categories)
    {
        m_upworkSubcategories.insert(category->categoryId(), category->subcategories());
        ui->upworkCategoryComboBox->addItem(category->title(), category->categoryId());
        if (category->title() == m_settings->upworkCategory())
        {
            ui->upworkCategoryComboBox->setCurrentIndex(ui->upworkCategoryComboBox->count() - 1);
        }
    }

    fillUpworkSubcategories(ui->upworkCategoryComboBox->currentIndex(), true);

    connect(ui->upworkCategoryComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::updateUpworkSubcategories);

    updateUpworkSearchButtonState();
}

void MainWindow::fillUpworkSubcategories(int categoryIndex, bool loadSettings)
{
    QVariant category = ui->upworkCategoryComboBox->itemData(categoryIndex);
    if (category.isValid())
    {
        QStringList subcategories = loadSettings ? m_settings->upworkSubcategories()
                                                 : QStringList();
        QString categoryId = category.toString();
        foreach (auto subcategory, m_upworkSubcategories.value(categoryId))
        {
            QListWidgetItem * item = new QListWidgetItem(subcategory->title());
            item->setData(Qt::UserRole, subcategory->categoryId());
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            if (loadSettings && subcategories.contains(subcategory->title()))
            {
                item->setCheckState(Qt::Checked);
            }
            else
            {
                item->setCheckState(Qt::Unchecked);
            }
            ui->upworkSubcategoryListWidget->addItem(item);
        }
    }
}

void MainWindow::updateUpworkSubcategories(int categoryIndex)
{
    ui->upworkSubcategoryListWidget->clear();
    ui->upworkSubcategoryListWidget->verticalScrollBar()->setValue(0);
    fillUpworkSubcategories(categoryIndex, false);
}

void MainWindow::searchUpworkJobs()
{
    ui->upworkSearchButton->setEnabled(false);
    ui->statusBar->showMessage(tr("Search for jobs..."));
    ui->upworkJobListWidget->clear();
    UpworkJobSearcher * searcher = new UpworkJobSearcher(upworkSearchJobParameters(),
                                                         m_upworkApiClient,
                                                         ui->upworkJobListWidget,
                                                         this);
    connect(searcher, &JobSearcher::searchFinished, this, &MainWindow::finishUpworkJobSearch);
    connect(searcher, &JobSearcher::maxJobCountReached,
            this, &MainWindow::processUpworkMaxJobCount);
    searcher->search();
}

UpworkSearchJobParameters MainWindow::upworkSearchJobParameters() const
{
    PostedDateRange postedDate =
        static_cast<PostedDateRange>(ui->upworkPostedDateComboBox->currentIndex());
    JobType jobType = static_cast<JobType>(ui->upworkJobTypeComboBox->currentIndex());
    UpworkSearchJobParameters parameters(ui->upworkCategoryComboBox->currentText(),
                                         ui->upworkSearchLineEdit->text(),
                                         postedDate,
                                         jobType);
    for (int i = 0; i < ui->upworkSubcategoryListWidget->count(); ++i)
    {
        QListWidgetItem * item = ui->upworkSubcategoryListWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            parameters.addSubcategory(item->text());
        }
    }
    if (jobType == JobType::Fixed)
    {
        bool isOk;
        int minBudget = ui->upworkMinBudgetLineEdit->text().toInt(&isOk);
        if (isOk)
        {
            parameters.setMinBudget(minBudget);
        }
        int maxBudget = ui->upworkMaxBudgetLineEdit->text().toInt(&isOk);
        if (isOk)
        {
            if (parameters.minBudget() > maxBudget)
            {
                maxBudget = parameters.minBudget() + 1;
                ui->upworkMaxBudgetLineEdit->setText(QString::number(maxBudget));
            }
            parameters.setMaxBudget(maxBudget);
        }
    }

    m_settings->saveUpworkSettings(parameters);

    return parameters;
}

void MainWindow::updateUpworkSearchButtonState()
{
    bool enable = m_upworkApiClient->isLoggedIn() &&
                  ui->upworkCategoryComboBox->currentIndex() > -1;
    ui->upworkSearchButton->setEnabled(enable);
}

void MainWindow::finishUpworkJobSearch()
{
    UpworkJobSearcher * searcher = qobject_cast<UpworkJobSearcher *>(sender());
    Q_ASSERT(searcher);
    searcher->deleteLater();

    ui->statusBar->clearMessage();
    updateUpworkSearchButtonState();
}

void MainWindow::processUpworkMaxJobCount(int count)
{
    QMessageBox::information(this,
                             tr("Attention"),
                             tr("Too many jobs were found. Searching was stopped after first %1 jobs loading.").arg(count));
}

void MainWindow::openUpworkJob(const QSharedPointer<Job> & job) const
{
    QDesktopServices::openUrl(job->url());
}

void MainWindow::logOutFromUpwork()
{
    updateUpworkActions(false);
    updateUpworkSearchButtonState();
    m_upworkApiClient->logOut();
    ui->upworkJobListWidget->clear();
}

void MainWindow::updateUpworkActions(bool isLoggedIn)
{
    ui->actionUpworkLogIn->setVisible(!isLoggedIn);
    ui->actionUpworkLogOut->setVisible(isLoggedIn);
}

void MainWindow::restoreWindow()
{
    m_trayIcon->hide();
    show();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        restoreWindow();
        break;
    }
}
