#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "settings.h"
#include "Upwork/upworkapiclient.h"

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

    m_upworkApiClient->initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_settings;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    //saveSettings();
    event->accept();
}

void MainWindow::setupConnections()
{
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
    connect(m_upworkApiClient, &UpworkApiClient::error, this, &MainWindow::processUpworkError);
    connect(m_upworkApiClient, &UpworkApiClient::initialized,
            this, &MainWindow::loadUpworkCategories);
}

void MainWindow::showAbout()
{
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::processUpworkError(UpworkApiError error)
{
    QString message;
    switch (error)
    {
    case UpworkApiError::ConnectionError:
        message = tr("Connection error. Please check your internet connection.");
        break;
    case UpworkApiError::ServiceError:
        message = tr("Upwork service is unavailable. Please try again later.");
        break;
    case UpworkApiError::UnknownError:
        message = tr("Unknown error");
        break;
    }
    QMessageBox::critical(this, tr("Error"), message);
}

void MainWindow::loadUpworkCategories()
{
    m_upworkApiClient->loadCategories();
}
