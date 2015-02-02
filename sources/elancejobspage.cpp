#include "elancejobspage.h"
#include "ielancejob.h"
#include "elancejob.h"

using namespace FreelanceNavigator;

ElanceJobsPage::ElanceJobsPage() : m_jobsTotal(-1), m_pagesTotal(-1), m_page(-1)
{
}

ElanceJobsPage::~ElanceJobsPage()
{
}

bool ElanceJobsPage::isValid() const
{
    return m_jobsTotal != -1 && m_pagesTotal != -1 && m_page != -1;
}

int ElanceJobsPage::jobsTotal() const
{
    return m_jobsTotal;
}

void ElanceJobsPage::setJobsTotal(int jobsTotal)
{
    m_jobsTotal = jobsTotal;
}

int ElanceJobsPage::pagesTotal() const
{
    return m_pagesTotal;
}

void ElanceJobsPage::setPagesTotal(int pagesTotal)
{
    m_pagesTotal = pagesTotal;
}

int ElanceJobsPage::page() const
{
    return m_page;
}

void ElanceJobsPage::setPage(int page)
{
    m_page = page;
}

const QList<QSharedPointer<IElanceJob> > & ElanceJobsPage::jobs() const
{
    return m_jobs;
}

void ElanceJobsPage::addJob(const QSharedPointer<IElanceJob> & job)
{
    m_jobs.append(job);
}
