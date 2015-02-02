#include "jobsloader.h"
#include "ielancejobspage.h"

using namespace FreelanceNavigator;

JobsLoader::JobsLoader(ElanceApiClient * elanceApiClient, QObject * parent)
    : QObject(parent),
      m_elanceApiClient(elanceApiClient)
{
    connect(m_elanceApiClient, &ElanceApiClient::jobsLoaded, this, &processLoadedJobs);
    connect(m_elanceApiClient, &ElanceApiClient::error, this, &processLoadError);
}

JobsLoader::~JobsLoader()
{
}

void JobsLoader::load(int category, const QList<int> & subcategories, JobType jobType, int page)
{
    m_category = category;
    m_subcategories = subcategories;
    m_jobType = jobType;
    m_requestedPage = page;
    m_currentPage = 1;
    m_jobs.clear();
    m_elanceApiClient->loadJobs(m_category, m_subcategories, 1);
}

const QList<QSharedPointer<IElanceJob> > & JobsLoader::jobs() const
{
    return m_jobs;
}

int JobsLoader::currentPage() const
{
    return m_currentPage;
}

void JobsLoader::processLoadedJobs(const QSharedPointer<IElanceJobsPage> & jobsPage)
{
    foreach (const QSharedPointer<IElanceJob> & job, jobsPage->jobs())
    {
        if (checkJob(job))
        {
            if (m_jobs.count() == m_pageSize)
            {
                // Start to fill next page;
                ++m_currentPage;
                m_jobs.clear();
            }
            m_jobs.append(job);
            if (m_jobs.count() == m_pageSize && m_currentPage == m_requestedPage)
            {
                // Requested page is fully loaded.
                emit loaded(true);
                return;
            }
        }
    }
    if (jobsPage->page() == jobsPage->pagesTotal())
    {
        // There are no available pages in the service for current request.
        emit loaded(true);
        return;
    }
    m_elanceApiClient->loadJobs(m_category, m_subcategories, jobsPage->page() + 1);
}

void JobsLoader::processLoadError(ElanceApiClient::ElanceApiError) const
{
    emit loaded(false);
}

bool JobsLoader::checkJob(const QSharedPointer<IElanceJob> & job) const
{
    if (m_jobType == JobsLoader::FixedPrice && job->isHourly())
    {
        return false;
    }
    if (m_jobType == JobsLoader::Hourly && !job->isHourly())
    {
        return false;
    }
    return true;
}
