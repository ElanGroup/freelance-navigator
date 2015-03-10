#include "jobsmanager.h"

using namespace FreelanceNavigator;

JobsManager::JobsManager(QObject * parent) : QObject(parent)
{
}

JobsManager::~JobsManager()
{
}

void JobsManager::clear()
{
    m_jobs.clear();
    m_pageJobs.clear();
}

int JobsManager::pagesTotal() const
{
    return m_jobs.count();
}

QList<QSharedPointer<IElanceJob> > JobsManager::getJobs(int page) const
{
    Q_ASSERT(m_jobs.contains(page));
    return m_jobs.value(page);
}

void JobsManager::processLoadedJobs(QList<QSharedPointer<IElanceJob> > jobs)
{
    foreach (const QSharedPointer<IElanceJob> & job, jobs)
    {
        m_pageJobs.append(job);
        if (m_pageJobs.count() == m_pageSize)
        {
            addJobsPage();
        }
    }
}

void JobsManager::processLoadFinish()
{
    if (!m_pageJobs.isEmpty())
    {
        addJobsPage();
    }
}

void JobsManager::addJobsPage()
{
    int page = m_jobs.isEmpty() ? 1 : m_jobs.lastKey() + 1;
    m_jobs.insert(page, m_pageJobs);
    m_pageJobs.clear();
    emit pageAdded();
}
