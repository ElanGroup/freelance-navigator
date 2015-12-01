#include <QWidget>
#include "joblistmodel.h"
#include "job.h"
#include "jobitem.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Widgets;

JobListModel::JobListModel(QWidget * parent) :
    QAbstractListModel(parent),
    m_paintDevice(parent)
{
}

int JobListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_jobList.count();
}

QVariant JobListModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.row() >= m_jobList.count() || index.row() < 0)
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        return QVariant::fromValue(m_jobList.at(index.row()));
    }

    return QVariant();
}

void JobListModel::addJobs(const QList<QSharedPointer<Job>> & jobs)
{
    int jobCount = m_jobList.count();
    beginInsertRows(QModelIndex(), jobCount, jobCount + jobs.count() - 1);
    foreach (const QSharedPointer<Job> & job, jobs)
    {
        m_jobList.append(QSharedPointer<JobItem>(new JobItem(job, m_paintDevice)));
    }
    endInsertRows();

}

void JobListModel::clear()
{
    beginResetModel();
    m_jobList.clear();
    endResetModel();
}
