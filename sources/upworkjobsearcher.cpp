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
    addJobsToListWidget(jobs);
}

void UpworkJobSearcher::stopSearch()
{
    disconnect(m_upworkApiClient, &UpworkApiClient::jobSearchingFinished,
               this, &JobSearcher::searchFinished);
    m_upworkApiClient->stopSearchJobs();

}
