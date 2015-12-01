#include "joblistmodel.h"
#include "job.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Widgets;

JobListModel::JobListModel(QObject * parent) : QAbstractListModel(parent)
{
}

int JobListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_jobCount;
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

void JobListModel::addJobs(const QList<QSharedPointer<Job> > & jobs)
{
    m_jobList.append(jobs);
}

void JobListModel::clear()
{
    beginResetModel();
    m_jobList.clear();
    m_jobCount = 0;
    endResetModel();
}

bool JobListModel::canFetchMore(const QModelIndex & parent) const
{
    Q_UNUSED(parent)
    return m_jobCount < m_jobList.count();
}

void JobListModel::fetchMore(const QModelIndex & parent)
{
    Q_UNUSED(parent)

    int remainder = m_jobList.count() - m_jobCount;
    int itemsToFetch = qMin(10, remainder);

    beginInsertRows(QModelIndex(), m_jobCount, m_jobCount + itemsToFetch - 1);
    m_jobCount += itemsToFetch;
    endInsertRows();
}
