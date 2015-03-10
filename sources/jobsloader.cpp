#include <QDateTime>
#include "jobsloader.h"
#include "ielancejobspage.h"

using namespace FreelanceNavigator;

JobsLoader::JobsLoader(ElanceApiClient * elanceApiClient, QObject * parent)
    : QObject(parent),
      m_elanceApiClient(elanceApiClient),
      m_category(-1),
      m_jobType(JobType::Any),
      m_postedDateRange(PostedDateRange::Any)
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

void JobsLoader::setJobType(JobType::Enum jobType)
{
    m_jobType = jobType;
}

void JobsLoader::setPostedDateRange(PostedDateRange::Enum postedDateRange)
{
    m_postedDateRange = postedDateRange;
}

void JobsLoader::load()
{
    m_elanceApiClient->loadJobs(m_category, m_subcategories, 1);
}

void JobsLoader::processLoadedJobs(const QSharedPointer<IElanceJobsPage> & jobsPage)
{
    QList<QSharedPointer<IElanceJob> > jobs;
    foreach (const QSharedPointer<IElanceJob> & job, jobsPage->jobs())
    {
        if (checkJob(job))
        {
            jobs.append(job);
        }
    }
    if (!jobs.isEmpty())
    {
        emit loaded(jobs);
    }
    if (jobsPage->page() >= jobsPage->pagesTotal())
    {
        // There are no available pages in the service for current request.
        emit loadFinished();
        return;
    }
    m_elanceApiClient->loadJobs(m_category, m_subcategories, jobsPage->page() + 1);
}

void JobsLoader::processLoadError(ElanceApiClient::ElanceApiError) const
{
    emit loadFinished();
}

bool JobsLoader::checkJob(const QSharedPointer<IElanceJob> & job) const
{
    return checkJobType(job) && checkPostedDate(job);
}

bool JobsLoader::checkJobType(const QSharedPointer<IElanceJob> & job) const
{
    switch (m_jobType)
    {
    case JobType::FixedPrice:
        return !job->isHourly();
    case JobType::Hourly:
        return job->isHourly();
    default:
        return true;
    }
}

bool JobsLoader::checkPostedDate(const QSharedPointer<IElanceJob> & job) const
{
    switch (m_postedDateRange)
    {
    case PostedDateRange::Day:
        return checkIsInRange(job->postedDate(), 1);
    case PostedDateRange::ThreeDays:
        return checkIsInRange(job->postedDate(), 3);
    case PostedDateRange::FiveDays:
        return checkIsInRange(job->postedDate(), 5);
    case PostedDateRange::Week:
        return checkIsInRange(job->postedDate(), 7);
    case PostedDateRange::TenDays:
        return checkIsInRange(job->postedDate(), 10);
    case PostedDateRange::TwoWeeks:
        return checkIsInRange(job->postedDate(), 14);
    case PostedDateRange::Month:
        return job->postedDate().addMonths(1) >= QDateTime::currentDateTimeUtc();
    default:
        return true;
    }
}

bool JobsLoader::checkIsInRange(const QDateTime & dateTime, int days)
{
    return dateTime.secsTo(QDateTime::currentDateTimeUtc()) <= days * m_dayLength;
}
