#include "jobsloader.h"
#include "ielancejobspage.h"

using namespace FreelanceNavigator;

JobsLoader::JobsLoader(ElanceApiClient * elanceApiClient, QObject * parent)
    : QObject(parent),
      m_elanceApiClient(elanceApiClient),
      m_category(-1),
      m_jobType(JobsLoader::Any),
      m_areMoreJobsAvailable(false)
{
    connect(m_elanceApiClient, &ElanceApiClient::jobsLoaded, this, &processLoadedJobs);
    connect(m_elanceApiClient, &ElanceApiClient::error, this, &processLoadError);
}

JobsLoader::~JobsLoader()
{
}

void JobsLoader::setCategory(int category)
{
    m_category = category;
}

void JobsLoader::setSubcategories(const QList<int> & subcategories)
{
    m_subcategories = subcategories;
}

void JobsLoader::setJobType(JobType jobType)
{
    m_jobType = jobType;
}

void JobsLoader::load(int page)
{
    m_areMoreJobsAvailable = false;
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

bool JobsLoader::areMoreJobsAvailable() const
{
    return m_areMoreJobsAvailable;
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
                m_areMoreJobsAvailable = true;
                emit loaded(true);
                return;
            }
        }
    }
    if (jobsPage->page() >= jobsPage->pagesTotal())
    {
        // There are no available pages in the service for current request.
        m_areMoreJobsAvailable = false;
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
