#include <QDebug>
#include <QDateTime>
#include "jobsloader.h"
#include "ielancejobspage.h"

using namespace FreelanceNavigator;

JobsLoader::JobsLoader(ElanceApiClient * elanceApiClient, QObject * parent)
    : QObject(parent),
      m_elanceApiClient(elanceApiClient),
      m_category(-1),
      m_jobType(JobType::Any),
      m_postedDateRange(PostedDateRange::Any),
      m_minBudget(-1),
      m_maxBudget(-1),
      m_includeNotSure(false),
      m_lastPage(-1)
{
    connect(m_elanceApiClient, &ElanceApiClient::jobsLoaded, this, &JobsLoader::processLoadedJobs);
    connect(m_elanceApiClient, &ElanceApiClient::error, this, &JobsLoader::processLoadError);
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

void JobsLoader::setBudget(int min, int max, bool includeNotSure)
{
    m_minBudget = min;
    m_maxBudget = max;
    m_includeNotSure = includeNotSure;
}

void JobsLoader::setKeywords(const QString & keywords)
{
    m_keywords = keywords;
}

void JobsLoader::load()
{
    m_lastPage = -1;
    m_jobs.clear();
    m_elanceApiClient->loadJobs(m_category, m_subcategories, m_keywords, 1);
}

void JobsLoader::processLoadedJobs(const QSharedPointer<IElanceJobsPage> & jobsPage)
{
    if (m_lastPage == -1 && jobsPage->pagesTotal() > 1)
    {
        for (int i = 2; i <= jobsPage->pagesTotal(); ++i)
        {
            m_elanceApiClient->loadJobs(m_category, m_subcategories, m_keywords, i);
        }
    }

    QList<QSharedPointer<IElanceJob> > jobs;
    foreach (const QSharedPointer<IElanceJob> & job, jobsPage->jobs())
    {
        if (checkJob(job))
        {
            jobs.append(job);
        }
    }

    if (m_lastPage == -1 || jobsPage->page() == m_lastPage + 1)
    {
        m_lastPage = jobsPage->page();
        int page = jobsPage->page() + 1;
        while (m_jobs.contains(page))
        {
            jobs.append(m_jobs.take(page));
            m_lastPage = page;
            ++page;
        }
        if (!jobs.isEmpty())
        {
            emit loaded(jobs);
        }
    }
    else
    {
        m_jobs.insert(jobsPage->page(), jobs);
    }

    if (jobsPage->page() >= jobsPage->pagesTotal())
    {
        // There are no available pages in the service for current request.
        emit loadFinished();
    }
}

void JobsLoader::processLoadError(ElanceApiClient::ElanceApiError) const
{
    emit loadFinished();
}

bool JobsLoader::checkJob(const QSharedPointer<IElanceJob> & job) const
{
    return checkJobType(job) && checkPostedDate(job) && checkBudget(job);
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

bool JobsLoader::checkBudget(const QSharedPointer<IElanceJob> & job) const
{
    if (m_minBudget == -1 && m_maxBudget == -1)
    {
        return true;
    }

    if (job->budget().toLower() == "not sure")
    {
        return m_includeNotSure;
    }

    double jobMinBudget;
    double jobMaxBudget;
    bool isParsed = parseBudget(job->budget(), jobMinBudget, jobMaxBudget);
    if (!isParsed || (jobMinBudget == -1 && jobMaxBudget == -1))
    {
        return false;
    }

    if (jobMinBudget == -1)
    {
        return m_minBudget == -1 ? true : m_minBudget <= jobMaxBudget;
    }

    if (jobMaxBudget == -1)
    {
        return m_maxBudget == -1 ? true : m_maxBudget >= jobMinBudget;
    }

    if (m_minBudget == -1)
    {
        return m_maxBudget >= jobMinBudget;
    }

    if (m_maxBudget == -1)
    {
        return m_minBudget <= jobMaxBudget;
    }

    return m_minBudget <= jobMaxBudget && m_maxBudget >= jobMinBudget;
}

bool JobsLoader::parseBudget(const QString & text, double & minBudget, double & maxBudget)
{
    minBudget = -1;
    maxBudget = -1;

    QStringList parts = text.toLower().split(QChar(' '), QString::SkipEmptyParts);

    if (parts.count() >= 3)
    {
        if (parts[0] == "less" && parts[1] == "than")
        {
            double max = parseCost(parts[2]);
            Q_ASSERT(max > 0);
            if (max > 0)
            {
                maxBudget = max - 0.01;
                return true;
            }
            return false;
        }
        if (parts[0] == "more" && parts[1] == "than")
        {
            double min = parseCost(parts[2]);
            Q_ASSERT(min > 0);
            if (min > 0)
            {
                minBudget = min + 0.01;
                return true;
            }
            return false;
        }
        if (parts[1] == "-")
        {
            double min = parseCost(parts[0]);
            double max = parseCost(parts[2]);
            Q_ASSERT(min > 0 && max > 0);
            if (min > 0 && max > 0)
            {
                minBudget = min;
                maxBudget = max;
                return true;
            }
            return false;
        }
        if (parts[parts.count() - 2] == "or" && parts[parts.count() - 1] == "less")
        {
            double max = parseCost(parts[0]);
            Q_ASSERT(max > 0);
            if (max > 0)
            {
                maxBudget = max;
                return true;
            }
            return false;
        }
        if (parts[parts.count() - 2] == "or" && parts[parts.count() - 1] == "more")
        {
            double min = parseCost(parts[0]);
            Q_ASSERT(min > 0);
            if (min > 0)
            {
                minBudget = min;
                return true;
            }
            return false;
        }
    }
    if (parts.count() >= 2 && parts[0] == "about")
    {
        double cost = parseCost(parts[1]);
        Q_ASSERT(cost > 0);
        if (cost > 0)
        {
            minBudget = cost;
            maxBudget = cost;
            return true;
        }
        return false;
    }

    Q_ASSERT(false);
    return false;
}

double JobsLoader::parseCost(QString & text)
{
    return text.remove(QRegExp("[$,]")).toDouble();
}
