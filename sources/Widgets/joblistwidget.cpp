#include <QStandardItemModel>
#include <QWidget>
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
    ui->jobListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->jobListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->jobListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->jobListView->setResizeMode(QListView::Adjust);
}

JobListWidget::~JobListWidget()
{
    delete ui;
}

void JobListWidget::clear()
{
    m_jobs.clear();
    m_jobModel->removeRows(0, m_jobModel->rowCount());
}

void JobListWidget::addJobs(const QList<QSharedPointer<Job>> & jobs)
{
    m_jobs.append(jobs);
    foreach (const QSharedPointer<Job> & job, jobs)
    {
        QStandardItem * item = new QStandardItem();
        item->setData(QVariant::fromValue(job), Qt::DisplayRole);
        m_jobModel->appendRow(item);
    }
}

void JobListWidget::resizeEvent(QResizeEvent * event)
{
    Q_UNUSED(event)

    // Force job items size recalculation on widget resize.
    ui->jobListView->doItemsLayout();
}
