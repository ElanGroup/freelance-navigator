#include "upworkjobpage.h"
#include "job.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Upwork;

UpworkJobPage::UpworkJobPage()
{
}

QList<QSharedPointer<Job>> UpworkJobPage::jobs() const
{
    return m_jobs;
}

void UpworkJobPage::addJob(const QSharedPointer<Job> & job)
{
    m_jobs.append(job);
}

int UpworkJobPage::offset() const
{
    return m_offset;
}

void UpworkJobPage::setOffset(int offset)
{
    m_offset = offset;
}

int UpworkJobPage::count() const
{
    return m_count;
}

void UpworkJobPage::setCount(int count)
{
    m_count = count;
}

int UpworkJobPage::total() const
{
    return m_total;
}

void UpworkJobPage::setTotal(int total)
{
    m_total = total;
}

bool UpworkJobPage::isValid() const
{
    return m_offset != -1 && m_count != -1 && m_total != -1;
}
