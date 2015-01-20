#include <QDebug>
#include <QScrollBar>
#include <QCloseEvent>
#include <QSettings>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "elanceapiclient.h"
#include "ielancecategory.h"
#include "ielancejobspage.h"
#include "ielancejob.h"
#include "elancesettingsdialog.h"
#include "aboutdialog.h"

using namespace FreelanceNavigator;

MainWindow::MainWindow(ElanceApiClient * elanceApiClient, QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_elanceApiClient(elanceApiClient)
{
    ui->setupUi(this);
    setWindowState(windowState() | Qt::WindowMaximized);

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
    m_elanceApiClient->loadJobs();
}

void MainWindow::showJobs(const QSharedPointer<IElanceJobsPage> & jobs)
{

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
