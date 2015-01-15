#include <QDebug>
#include <QScrollBar>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "elanceapiclient.h"
#include "ielancecategory.h"
#include "ielancejobspage.h"
#include "ielancejob.h"

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
    m_elanceApiClient->loadJobs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showCategories(const QList<QSharedPointer<IElanceCategory> > & categories)
{
    foreach (const QSharedPointer<IElanceCategory> & category, categories)
    {
        m_subcategories.insert(category->categoryId(), category->subcategories());
        ui->categoriesComboBox->addItem(category->name(), category->categoryId());
    }
}

void MainWindow::showJobs(const QSharedPointer<IElanceJobsPage> & jobs)
{

}

void MainWindow::updateSubcategories(int categoryIndex)
{
    QVariant category = ui->categoriesComboBox->itemData(categoryIndex);
    if (category.isValid())
    {
        int categoryId = category.toInt();
        ui->subcategoriesListWidget->clear();
        ui->subcategoriesListWidget->verticalScrollBar()->setValue(0);
        foreach (const QSharedPointer<IElanceCategory> & subcategory,
                 m_subcategories.value(categoryId))
        {
            QListWidgetItem * item = new QListWidgetItem(subcategory->name());
            item->setData(Qt::UserRole, subcategory->categoryId());
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
            ui->subcategoriesListWidget->addItem(item);
        }
    }
}

void MainWindow::setupConnections()
{
    connect(m_elanceApiClient, &ElanceApiClient::categoriesLoaded,
            this, &MainWindow::showCategories);
    connect(m_elanceApiClient, &ElanceApiClient::jobsLoaded, this, &MainWindow::showJobs);
    connect(ui->categoriesComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::updateSubcategories);
}
