#include <QDebug>
#include <QCloseEvent>
#include <QStandardItemModel>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

using namespace FreelanceNavigator;

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_jobsModel(new QStandardItemModel(0, 1, this))
{
    ui->setupUi(this);
    setWindowState(windowState() | Qt::WindowMaximized);
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
