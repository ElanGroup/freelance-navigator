#include "jobsearcher.h"
#include "Widgets/joblistwidget.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Widgets;

JobSearcher::JobSearcher(JobListWidget * jobListWidget, QObject * parent) :
    QObject(parent),
    m_jobListWidget(jobListWidget)
{
}

JobSearcher::~JobSearcher()
{
}

void JobSearcher::addJobsToListWidget(const QList<QSharedPointer<Job>> & jobs)
{
    if (jobs.isEmpty())
    {
        return;
    }
    m_jobListWidget->addJobs(jobs);
    m_jobCount += jobs.count();
    if (m_jobCount >= m_maxJobCount)
    {
        stopSearch();
        emit maxJobCountReached(m_maxJobCount);
        emit searchFinished();
    }
}


