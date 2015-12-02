#include "upworkjob.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Upwork;

UpworkJob::UpworkJob()
{
}

QString UpworkJob::jobId() const
{
    return m_jobId;
}

void UpworkJob::setJobId(const QString & jobId)
{
    m_jobId = jobId;
}

QString UpworkJob::title() const
{
    return m_title;
}

void UpworkJob::setTitle(const QString & title)
{
    m_title = title;
}

QString UpworkJob::description() const
{
    return m_description;
}

void UpworkJob::setDescription(const QString & description)
{
    m_description = description;
}

JobType UpworkJob::type() const
{
    return m_type;
}

void UpworkJob::setType(JobType type)
{
    m_type = type;
}

int UpworkJob::budget() const
{
    return m_budget;
}

void UpworkJob::setBudget(int budget)
{
    m_budget = budget;
}

const QDateTime & UpworkJob::postedDate() const
{
    return m_postedDate;
}

void UpworkJob::setPostedDate(const QDateTime & postedDate)
{
    m_postedDate = postedDate;
}
