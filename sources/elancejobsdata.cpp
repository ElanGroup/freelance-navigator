#include "elancejobsdata.h"
#include "ielancejobdata.h"
#include "elancejobdata.h"

using namespace FreelanceNavigator;

ElanceJobsData::ElanceJobsData()
{
}

ElanceJobsData::~ElanceJobsData()
{
}

int ElanceJobsData::jobsTotal() const
{
    return m_jobsTotal;
}

void ElanceJobsData::setJobsTotal(int jobsTotal)
{
    m_jobsTotal = jobsTotal;
}

const QList<QSharedPointer<IElanceJobData> > & ElanceJobsData::jobs() const
{
    return m_jobs;
}

void ElanceJobsData::addJob(ElanceJobData * job)
{
    m_jobs.append(QSharedPointer<IElanceJobData>(job));
}
