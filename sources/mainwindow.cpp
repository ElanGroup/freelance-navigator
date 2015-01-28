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

using namespace FreelanceNavigator;

MainWindow::MainWindow(ElanceApiClient * elanceApiClient, QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_elanceApiClient(elanceApiClient),
    m_jobsModel(new QStandardItemModel(0, 1, this))
{
    ui->setupUi(this);
    setWindowState(windowState() | Qt::WindowMaximized);
    ui->jobsListView->setModel(m_jobsModel);
    ui->jobsListView->setItemDelegate(new JobItemDelegate(this));

    setupConnections();

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

void MainWindow::loadJobs()
{
    ui->loadJobsButton->setEnabled(false);
    m_jobsModel->removeRows(0, m_jobsModel->rowCount());
    int category = ui->categoriesComboBox->currentData().toInt();
    QList<int> subcategories;
    for (int i = 0; i < ui->subcategoriesListWidget->count(); ++i)
    {
        QListWidgetItem * item = ui->subcategoriesListWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            subcategories.append(item->data(Qt::UserRole).toInt());
        }
    }
    m_elanceApiClient->loadJobs(category, subcategories);
}

void MainWindow::showJobs(const QSharedPointer<IElanceJobsPage> & jobs)
{
    foreach (const QSharedPointer<IElanceJob> & job, jobs->jobs())
    {
        QStandardItem * item = new QStandardItem();
        item->setData(QVariant::fromValue(job), Qt::DisplayRole);
        m_jobsModel->appendRow(item);
    }
    ui->loadJobsButton->setEnabled(true);
}

void MainWindow::processError(ElanceApiClient::ElanceApiError error)
{
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
    ui->loadJobsButton->setEnabled(true);
}

void MainWindow::setupConnections()
{
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionElanceAPISettings, &QAction::triggered,
            this, &MainWindow::editElanceSettings);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(ui->loadJobsButton, &QPushButton::clicked, this, &MainWindow::loadJobs);
    connect(m_elanceApiClient, &ElanceApiClient::categoriesLoaded,
            this, &MainWindow::fillCategories);
    connect(m_elanceApiClient, &ElanceApiClient::jobsLoaded, this, &MainWindow::showJobs);
    connect(m_elanceApiClient, &ElanceApiClient::error, this, &MainWindow::processError);
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
}
