#include <QDebug>
#include <QStandardItemModel>
#include <QListView>
#include <QVBoxLayout>
#include "joblistwidget.h"
#include "job.h"
#include "jobitemdelegate.h"
#include "joblistmodel.h"
#include "jobitem.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Widgets;

JobListWidget::JobListWidget(QWidget * parent) :
    QWidget(parent),
    m_jobListView(new QListView(this)),
    m_jobListModel(new JobListModel(this))
{
    m_jobListView->setModel(m_jobListModel);
    m_jobListView->setItemDelegate(new JobItemDelegate(this));
    m_jobListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_jobListView->setResizeMode(QListView::Adjust);

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->addWidget(m_jobListView);
    setLayout(layout);

    connect(m_jobListView, &QListView::doubleClicked, this, &JobListWidget::onDoubleClick);
}

JobListWidget::~JobListWidget()
{
}

void JobListWidget::clear()
{
    m_jobListModel->clear();
}

void JobListWidget::addJobs(const QList<QSharedPointer<Job>> & jobs)
{
    m_jobListModel->addJobs(jobs);
}

void JobListWidget::resizeEvent(QResizeEvent * event)
{
    Q_UNUSED(event)

    // Force job items size recalculation on widget resize.
    m_jobListView->doItemsLayout();
}

void JobListWidget::onDoubleClick(const QModelIndex & index)
{
    QSharedPointer<JobItem> jobItem = qvariant_cast<QSharedPointer<JobItem>>(index.data());
    Q_ASSERT(jobItem);

    emit jobOpenned(jobItem->job());
}
