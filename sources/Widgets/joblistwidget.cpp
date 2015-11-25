#include <QStandardItemModel>
#include "joblistwidget.h"
#include "ui_joblistwidget.h"
#include "job.h"
#include "jobitemdelegate.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Widgets;

JobListWidget::JobListWidget(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::JobListWidget),
    m_jobModel(new QStandardItemModel(0, 1, this))
{
    ui->setupUi(this);
    ui->jobListView->setModel(m_jobModel);
    ui->jobListView->setItemDelegate(new JobItemDelegate(this));
}

JobListWidget::~JobListWidget()
{
    delete ui;
}

void JobListWidget::addJobs(const QList<QSharedPointer<Job>> & jobs)
{
    m_jobs.append(jobs);
}
