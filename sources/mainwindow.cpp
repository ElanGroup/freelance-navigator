#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "elanceapiclient.h"

using namespace FreelanceNavigator;

MainWindow::MainWindow(const ElanceApiClient & elanceApiClient, QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_elanceApiClient(elanceApiClient)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
