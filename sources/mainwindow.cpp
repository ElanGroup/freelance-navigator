#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "elanceapiclient.h"
#include "ielancejobsdata.h"

using namespace FreelanceNavigator;

MainWindow::MainWindow(ElanceApiClient * elanceApiClient, QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_elanceApiClient(elanceApiClient)
{
    ui->setupUi(this);

    setupConnections();

    m_elanceApiClient->loadJobs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showJobs(QSharedPointer<IElanceJobsData> jobsData)
{
    qDebug() << jobsData->jobsCount();
}

void MainWindow::setupConnections()
{
    connect(m_elanceApiClient, &ElanceApiClient::jobsLoaded, this, &MainWindow::showJobs);
}
