#include "joblistwidget.h"
#include "ui_joblistwidget.h"

using namespace FreelanceNavigator::Widgets;

JobListWidget::JobListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JobListWidget)
{
    ui->setupUi(this);
}

