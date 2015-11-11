#include <QDebug>
#include <QCloseEvent>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

using namespace FreelanceNavigator;

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(windowState() | Qt::WindowMaximized);

    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
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
}

void MainWindow::showAbout()
{
    AboutDialog dialog;
    dialog.exec();
}
