#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QScrollBar>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settings.h"
#include "Upwork/upworkapiclient.h"
#include "Upwork/upworkcategory.h"
#include "upworkjobsearcher.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Upwork;

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(new Settings()),
    m_upworkApiClient(new UpworkApiClient(m_settings, this))
{
    ui->setupUi(this);
    setWindowState(windowState() | Qt::WindowMaximized);

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

void MainWindow::setupConnections()
{
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->upworkSearchButton, &QPushButton::clicked, this, &MainWindow::searchUpworkJobs);
    connect(m_upworkApiClient, &UpworkApiClient::error, this, &MainWindow::processUpworkError);
    connect(m_upworkApiClient, &UpworkApiClient::warning, this, &MainWindow::processUpworkWarning);
    connect(m_upworkApiClient, &UpworkApiClient::initialized,
            this, &MainWindow::loadUpworkCategories);
    connect(m_upworkApiClient, &UpworkApiClient::categoriesLoaded,
            this, &MainWindow::fillUpworkCategories);
}

void MainWindow::setupUpworkFilters()
{
    ui->upworkPostedDateComboBox->setCurrentIndex(m_settings->upworkPostedDateRange());
    ui->upworkJobTypeComboBox->setCurrentIndex(m_settings->upworkJobType());
}

void MainWindow::showAbout()
{
    AboutDialog dialog;
    dialog.exec();
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
    QString message;
    switch (warning)
    {
    case UpworkApiWarning::AuthorizationRequired:
        message = tr("Upwork authorization was not finished. You should log in to Upwork service.");
        break;
    }
    QMessageBox::warning(this, tr("Warning"), message);
}

void MainWindow::loadUpworkCategories()
{
    ui->statusBar->showMessage(tr("Load categories..."));
    m_upworkApiClient->loadCategories();
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
    connect(searcher, &JobSearcher::maxJobCountReached, this, &MainWindow::processMaxJobCount);
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
    m_settings->saveUpworkSettings(parameters);
    return parameters;
}

void MainWindow::updateUpworkSearchButtonState()
{
    bool enable = ui->upworkCategoryComboBox->currentIndex() > -1;
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

void MainWindow::processMaxJobCount(int count)
{
    QMessageBox::information(this,
                             tr("Attention"),
                             tr("Too many jobs were found. Searching was stopped after first %1 jobs loading.").arg(count));
}
