#include <QDebug>
#include <QScrollBar>
#include <QCloseEvent>
#include <QSettings>
#include <QStandardItemModel>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ielancecategory.h"
#include "ielancejobspage.h"
#include "ielancejob.h"
#include "elancesettingsdialog.h"
#include "aboutdialog.h"
#include "jobitemdelegate.h"
#include "jobsloader.h"
#include "jobsmanager.h"

using namespace FreelanceNavigator;

MainWindow::MainWindow(ElanceApiClient * elanceApiClient, QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_elanceApiClient(elanceApiClient),
    m_jobsModel(new QStandardItemModel(0, 1, this)),
    m_jobsLoader(new JobsLoader(m_elanceApiClient, this)),
    m_jobsManager(new JobsManager(this)),
    m_currentJobsPage(0),
    m_minBudget(-1),
    m_maxBudget(-1),
    m_minHourlyRate(-1),
    m_maxHourlyRate(-1)
{
    ui->setupUi(this);
    setWindowState(windowState() | Qt::WindowMaximized);
    ui->jobsListView->setModel(m_jobsModel);
    ui->jobsListView->setItemDelegate(new JobItemDelegate(this));

    ui->minLineEdit->setValidator(new QIntValidator(0, 99999, this));
    ui->maxLineEdit->setValidator(new QIntValidator(0, 99999, this));

    setupConnections();
    loadSettings();

    jobTypeChanged(ui->jobTypesComboBox->currentIndex());

    ui->statusBar->showMessage(tr("Load categories..."));
    m_elanceApiClient->loadCategories();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    saveSettings();
    event->accept();
}

void MainWindow::fillCategories(const QList<QSharedPointer<IElanceCategory> > & categories)
{
    ui->statusBar->clearMessage();

    QSettings settings;
    settings.beginGroup("Settings");
    int savedCategoryId = settings.value("Category").toInt();

    foreach (const QSharedPointer<IElanceCategory> & category, categories)
    {
        m_subcategories.insert(category->categoryId(), category->subcategories());
        ui->categoriesComboBox->addItem(category->name(), category->categoryId());
        if (category->categoryId() == savedCategoryId)
        {
            ui->categoriesComboBox->setCurrentIndex(ui->categoriesComboBox->count() - 1);
        }
    }

    fillSubcategories(ui->categoriesComboBox->currentIndex(), true);

    connect(ui->categoriesComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::updateSubcategories);

    ui->loadJobsButton->setEnabled(categories.count() > 0);
}

void MainWindow::fillSubcategories(int categoryIndex, bool loadSettings)
{
    QList<int> savedSubcategories;
    if (loadSettings)
    {
        QSettings settings;
        settings.beginGroup("Settings");
        QList<QVariant> subcategories = settings.value("Subcategories").value<QList<QVariant> >();
        foreach (const QVariant & subcategory, subcategories)
        {
            bool isOk;
            int subcategoryId = subcategory.toInt(&isOk);
            if (isOk)
            {
                savedSubcategories.append(subcategoryId);
            }
        }
    }

    QVariant category = ui->categoriesComboBox->itemData(categoryIndex);
    if (category.isValid())
    {
        int categoryId = category.toInt();
        foreach (const QSharedPointer<IElanceCategory> & subcategory,
                 m_subcategories.value(categoryId))
        {
            QListWidgetItem * item = new QListWidgetItem(subcategory->name());
            item->setData(Qt::UserRole, subcategory->categoryId());
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            if (savedSubcategories.contains(subcategory->categoryId()))
            {
                item->setCheckState(Qt::Checked);
            }
            else
            {
                item->setCheckState(Qt::Unchecked);
            }
            ui->subcategoriesListWidget->addItem(item);
        }
    }
}

void MainWindow::updateSubcategories(int categoryIndex)
{
    ui->subcategoriesListWidget->clear();
    ui->subcategoriesListWidget->verticalScrollBar()->setValue(0);
    fillSubcategories(categoryIndex, false);
}

void MainWindow::editElanceSettings()
{
    ElanceSettingsDialog dialog;
    dialog.exec();
}

void MainWindow::showAbout()
{
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::logout()
{
    hide();
    m_elanceApiClient->logout();
    bool isAuthorized = m_elanceApiClient->authorize();
    if (isAuthorized)
    {
        m_jobsModel->removeRows(0, m_jobsModel->rowCount());
        show();
    }
    else
    {
        close();
    }
}

void MainWindow::jobTypeChanged(int index)
{
    switch (index)
    {
    case 1:
        ui->budgetLabel->setText(tr("Budget:"));
        ui->minLineEdit->setText(m_minBudget == -1 ? "" : QString::number(m_minBudget));
        ui->maxLineEdit->setText(m_maxBudget == -1 ? "" : QString::number(m_maxBudget));
        break;
    case 2:
        ui->budgetLabel->setText(tr("Hourly Rate:"));
        ui->minLineEdit->setText(m_minHourlyRate == -1 ? "" : QString::number(m_minHourlyRate));
        ui->maxLineEdit->setText(m_maxHourlyRate == -1 ? "" : QString::number(m_maxHourlyRate));
        break;
    }
    ui->budgetLabel->setVisible(index == 1 || index == 2);
    ui->minLabel->setVisible(index == 1 || index == 2);
    ui->minLineEdit->setVisible(index == 1 || index == 2);
    ui->maxLabel->setVisible(index == 1 || index == 2);
    ui->maxLineEdit->setVisible(index == 1 || index == 2);
    ui->notSuteCheckBox->setVisible(index == 1 || index == 2);
}

void MainWindow::loadJobs()
{
    m_jobsManager->clear();
    m_jobsLoader->setCategory(ui->categoriesComboBox->currentData().toInt());
    setSubcategoriesFilter();
    setJobTypeFilter();
    setPostedDateFilter();
    setBudgetFilter();
    m_jobsLoader->setKeywords(ui->keywordsLineEdit->text());
    m_currentJobsPage = 0;
    ui->loadJobsButton->setEnabled(false);
    ui->firstPageButton->setEnabled(false);
    ui->previousPageButton->setEnabled(false);
    ui->nextPageButton->setEnabled(false);
    ui->lastPageButton->setEnabled(false);
    ui->pageLineEdit->setText(QString::number(m_currentJobsPage));
    ui->pageLineEdit->setEnabled(false);
    ui->pageTotalLabel->setText("/ 0");
    ui->pageTotalLabel->setEnabled(false);
    m_jobsModel->removeRows(0, m_jobsModel->rowCount());
    ui->statusBar->showMessage(tr("Load jobs..."));
    m_jobsLoader->load();
}

void MainWindow::updateJobControls()
{
    if (m_currentJobsPage == 0)
    {
        showJobs(1);
        ui->pageLineEdit->setEnabled(true);
        ui->pageTotalLabel->setEnabled(true);
    }
    else
    {
        updatePageButtons();
    }
    ui->pageLineEdit->setValidator(new QIntValidator(1, m_jobsManager->pagesTotal(), this));
    ui->pageTotalLabel->setText("/ " + QString::number(m_jobsManager->pagesTotal()));
}

void MainWindow::finishJobsLoad()
{
    ui->statusBar->clearMessage();
    ui->loadJobsButton->setEnabled(true);
}

void MainWindow::showFirstPageOfJobs()
{
    showJobs(1);
}

void MainWindow::showPreviousPageOfJobs()
{
    showJobs(m_currentJobsPage - 1);
}

void MainWindow::showNextPageOfJobs()
{
    showJobs(m_currentJobsPage + 1);
}

void MainWindow::showLastPageOfJobs()
{
    showJobs(m_jobsManager->pagesTotal());
}

void MainWindow::showRequestedPageOfJobs()
{
    bool isOk;
    int page = ui->pageLineEdit->text().toInt(&isOk);
    if (isOk)
    {
        showJobs(page);
    }
}

void MainWindow::processError(ElanceApiClient::ElanceApiError error)
{
    ui->statusBar->clearMessage();
    QString message;
    switch (error)
    {
    case ElanceApiClient::ConnectionError:
        message = tr("Connection error. Please check your internet connection.");
        break;
    case ElanceApiClient::ServiceError:
        message = tr("Elance service is unavailable. Please try again later.");
        break;
    case ElanceApiClient::UnknownError:
        message = tr("Unknown error");
        break;
    }
    QMessageBox::critical(this, tr("Error"), message);
}

void MainWindow::setSubcategoriesFilter()
{
    QList<int> subcategories;
    for (int i = 0; i < ui->subcategoriesListWidget->count(); ++i)
    {
        QListWidgetItem * item = ui->subcategoriesListWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            subcategories.append(item->data(Qt::UserRole).toInt());
        }
    }
    m_jobsLoader->setSubcategories(subcategories);
}

void MainWindow::setJobTypeFilter()
{
    switch (ui->jobTypesComboBox->currentIndex())
    {
    case 1:
        m_jobsLoader->setJobType(JobType::FixedPrice);
        break;
    case 2:
        m_jobsLoader->setJobType(JobType::Hourly);
        break;
    default:
        m_jobsLoader->setJobType(JobType::Any);
        break;
    }
}

void MainWindow::setPostedDateFilter()
{
    switch (ui->postedDateComboBox->currentIndex())
    {
    case 1:
        m_jobsLoader->setPostedDateRange(PostedDateRange::Day);
        break;
    case 2:
        m_jobsLoader->setPostedDateRange(PostedDateRange::ThreeDays);
        break;
    case 3:
        m_jobsLoader->setPostedDateRange(PostedDateRange::FiveDays);
        break;
    case 4:
        m_jobsLoader->setPostedDateRange(PostedDateRange::Week);
        break;
    case 5:
        m_jobsLoader->setPostedDateRange(PostedDateRange::TenDays);
        break;
    case 6:
        m_jobsLoader->setPostedDateRange(PostedDateRange::TwoWeeks);
        break;
    case 7:
        m_jobsLoader->setPostedDateRange(PostedDateRange::Month);
        break;
    default:
        m_jobsLoader->setPostedDateRange(PostedDateRange::Any);
        break;
    }
}

void MainWindow::setBudgetFilter()
{
    if (ui->jobTypesComboBox->currentIndex() == 0)
    {
        m_minBudget = -1;
        m_maxBudget = -1;
        m_minHourlyRate = -1;
        m_maxHourlyRate = -1;
        m_jobsLoader->setBudget(-1, -1, true);
        return;
    }

    bool isMinOk;
    int min = ui->minLineEdit->text().toInt(&isMinOk);
    bool isMaxOk;
    int max = ui->maxLineEdit->text().toInt(&isMaxOk);

    if (isMinOk && isMaxOk && min > max)
    {
        max = min + 1;
        ui->maxLineEdit->setText(QString::number(max));
    }

    switch (ui->jobTypesComboBox->currentIndex())
    {
    case 1:
        m_minBudget = isMinOk ? min : -1;
        m_maxBudget = isMaxOk ? max : -1;
        m_minHourlyRate = -1;
        m_maxHourlyRate = -1;
        m_jobsLoader->setBudget(m_minBudget, m_maxBudget, ui->notSuteCheckBox->isChecked());
        break;
    case 2:
        m_minBudget = -1;
        m_maxBudget = -1;
        m_minHourlyRate = isMinOk ? min : -1;
        m_maxHourlyRate = isMaxOk ? max : -1;
        m_jobsLoader->setBudget(m_minHourlyRate,
                                m_maxHourlyRate,
                                ui->notSuteCheckBox->isChecked());
        break;
    }
}

void MainWindow::showJobs(int page)
{
    Q_ASSERT(page > 0 && page <= m_jobsManager->pagesTotal());
    m_currentJobsPage = page;
    ui->pageLineEdit->setText(QString::number(m_currentJobsPage));
    m_jobsModel->removeRows(0, m_jobsModel->rowCount());
    ui->jobsListView->scrollToTop();
    foreach (const QSharedPointer<IElanceJob> & job, m_jobsManager->getJobs(m_currentJobsPage))
    {
        QStandardItem * item = new QStandardItem();
        item->setData(QVariant::fromValue(job), Qt::DisplayRole);
        m_jobsModel->appendRow(item);
    }
    updatePageButtons();
}

void MainWindow::updatePageButtons()
{
    ui->firstPageButton->setEnabled(m_currentJobsPage > 1);
    ui->previousPageButton->setEnabled(m_currentJobsPage > 1);
    ui->nextPageButton->setEnabled(m_currentJobsPage < m_jobsManager->pagesTotal());
    ui->lastPageButton->setEnabled(m_currentJobsPage < m_jobsManager->pagesTotal());
}

void MainWindow::setupConnections()
{
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionElanceAPISettings, &QAction::triggered, this, &editElanceSettings);
    connect(ui->actionAbout, &QAction::triggered, this, &showAbout);
    connect(ui->actionLogout, &QAction::triggered, this, &logout);
    connect(ui->jobTypesComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &jobTypeChanged);
    connect(ui->loadJobsButton, &QPushButton::clicked, this, &loadJobs);
    connect(ui->firstPageButton, &QPushButton::clicked, this, &showFirstPageOfJobs);
    connect(ui->previousPageButton, &QPushButton::clicked, this, &showPreviousPageOfJobs);
    connect(ui->nextPageButton, &QPushButton::clicked, this, &showNextPageOfJobs);
    connect(ui->lastPageButton, &QPushButton::clicked, this, &showLastPageOfJobs);
    connect(ui->pageLineEdit, &QLineEdit::returnPressed, this, &showRequestedPageOfJobs);
    connect(m_elanceApiClient, &ElanceApiClient::categoriesLoaded, this, &fillCategories);
    connect(m_elanceApiClient, &ElanceApiClient::error, this, &processError);
    connect(m_jobsLoader, &JobsLoader::loaded, m_jobsManager, &JobsManager::processLoadedJobs);
    connect(m_jobsLoader, &JobsLoader::loadFinished,
            m_jobsManager, &JobsManager::processLoadFinish);
    connect(m_jobsLoader, &JobsLoader::loadFinished, this, &finishJobsLoad);
    connect(m_jobsManager, &JobsManager::pageAdded, this, &updateJobControls);
}

void MainWindow::loadSettings()
{
    QSettings settings;
    settings.beginGroup("Settings");

    QVariant jobTypeValue = settings.value("Job Type");
    if (jobTypeValue.isValid())
    {
        ui->jobTypesComboBox->setCurrentIndex(jobTypeValue.toInt());
    }

    QVariant postedDateValue = settings.value("Posted Date");
    if (postedDateValue.isValid())
    {
        ui->postedDateComboBox->setCurrentIndex(postedDateValue.toInt());
    }

    QVariant minBudgetValue = settings.value("Min Budget");
    if (minBudgetValue.isValid())
    {
        m_minBudget = minBudgetValue.toInt();
    }

    QVariant maxBudgetValue = settings.value("Max Budget");
    if (maxBudgetValue.isValid())
    {
        m_maxBudget = maxBudgetValue.toInt();
    }

    QVariant minHourlyRateValue = settings.value("Min Hourly Rate");
    if (minHourlyRateValue.isValid())
    {
        m_minHourlyRate = minHourlyRateValue.toInt();
    }

    QVariant maxHourlyRateValue = settings.value("Max Hourly Rate");
    if (maxHourlyRateValue.isValid())
    {
        m_maxHourlyRate = maxHourlyRateValue.toInt();
    }

    QVariant includeNotSureValue = settings.value("Include Not Sure");
    if (includeNotSureValue.isValid())
    {
        ui->notSuteCheckBox->setChecked(includeNotSureValue.toBool());
    }
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.beginGroup("Settings");

    QVariant category = ui->categoriesComboBox->currentData();
    if (category.isValid())
    {
        settings.setValue("Category", category);
    }

    QList<QVariant> subcategories;
    for (int i = 0; i < ui->subcategoriesListWidget->count(); ++i)
    {
        QListWidgetItem * item = ui->subcategoriesListWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            QVariant subcategory = item->data(Qt::UserRole);
            if (subcategory.isValid())
            {
                subcategories.append(subcategory);
            }
        }
    }
    settings.setValue("Subcategories", QVariant::fromValue(subcategories));

    settings.setValue("Job Type", ui->jobTypesComboBox->currentIndex());
    settings.setValue("Posted Date", ui->postedDateComboBox->currentIndex());
    settings.setValue("Min Budget", m_minBudget);
    settings.setValue("Max Budget", m_maxBudget);
    settings.setValue("Min Hourly Rate", m_minHourlyRate);
    settings.setValue("Max Hourly Rate", m_maxHourlyRate);
    settings.setValue("Include Not Sure", ui->notSuteCheckBox->isChecked());
}
