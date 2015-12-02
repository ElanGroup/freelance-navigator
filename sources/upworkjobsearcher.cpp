#include <QDateTime>
#include "upworkjobsearcher.h"
#include "Upwork/upworkapiclient.h"
#include "job.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Widgets;
using namespace FreelanceNavigator::Upwork;

UpworkJobSearcher::UpworkJobSearcher(UpworkSearchJobParameters && parameters,
                                     UpworkApiClient * upworkApiClient,
                                     JobListWidget * jobListWidget,
                                     QObject * parent) :
    JobSearcher(jobListWidget, parent),
    m_searchParameters(std::move(parameters)),
    m_upworkApiClient(upworkApiClient)
{
    connect(m_upworkApiClient, &UpworkApiClient::jobsLoaded,
            this, &UpworkJobSearcher::processFoundJobs);
    connect(m_upworkApiClient, &UpworkApiClient::jobSearchingFinished,
            this, &JobSearcher::searchFinished);
}

void UpworkJobSearcher::search() const
{
    m_upworkApiClient->searchJobs(m_searchParameters);
}

void UpworkJobSearcher::processFoundJobs(const QList<QSharedPointer<Job>> & jobs)
{
    addJobsToListWidget(filterJobs(jobs));
}

void UpworkJobSearcher::stopSearch()
{
    disconnect(m_upworkApiClient, &UpworkApiClient::jobSearchingFinished,
               this, &JobSearcher::searchFinished);
    m_upworkApiClient->stopSearchJobs();

}

QList<QSharedPointer<Job>> UpworkJobSearcher::filterJobs(const QList<QSharedPointer<Job>> & jobs) const
{
    QList<QSharedPointer<Job>> filteredJobs;
    foreach (const QSharedPointer<Job> & job, jobs)
    {
        if (checkPostedDate(job) && checkSearchQuery(job))
        {
            filteredJobs.append(job);
        }
    }
    return filteredJobs;
}

bool UpworkJobSearcher::checkPostedDate(const QSharedPointer<Job> & job) const
{
    switch (m_searchParameters.postedDateRange())
    {
    case PostedDateRange::Day:
        return checkDateRange(job->postedDate(), 1);
    case PostedDateRange::ThreeDays:
        return checkDateRange(job->postedDate(), 3);
    case PostedDateRange::FiveDays:
        return checkDateRange(job->postedDate(), 5);
    case PostedDateRange::Week:
        return checkDateRange(job->postedDate(), 7);
    case PostedDateRange::TenDays:
        return checkDateRange(job->postedDate(), 10);
    case PostedDateRange::TwoWeeks:
        return checkDateRange(job->postedDate(), 14);
    case PostedDateRange::Month:
        return job->postedDate().addMonths(1) >= QDateTime::currentDateTimeUtc();
    default:
        return true;
    }
}

bool UpworkJobSearcher::checkDateRange(const QDateTime & date, int days)
{
    return date.addDays(days) >= QDateTime::currentDateTimeUtc();
}

bool UpworkJobSearcher::checkSearchQuery(const QSharedPointer<Job> & job) const
{
    return m_searchParameters.searchQuery().isEmpty() ||
           job->title().contains(m_searchParameters.searchQuery(), Qt::CaseInsensitive) ||
           job->description().contains(m_searchParameters.searchQuery(), Qt::CaseInsensitive) ||
           job->skills().contains(m_searchParameters.searchQuery(), Qt::CaseInsensitive);
}
