#ifndef ELANCEJOBSPAGE_H
#define ELANCEJOBSPAGE_H

#include "ielancejobspage.h"

namespace FreelanceNavigator
{
class ElanceJobsPage : public IElanceJobsPage
{
public:
    ElanceJobsPage();
    ~ElanceJobsPage();

    virtual bool isValid() const;
    virtual int jobsTotal() const;
    void setJobsTotal(int jobsTotal);
    virtual const QList<QSharedPointer<IElanceJob> > & jobs() const;
    void addJob(const QSharedPointer<IElanceJob> & job);

private:
    ElanceJobsPage(const ElanceJobsPage &);
    ElanceJobsPage & operator=(const ElanceJobsPage &);

    int m_jobsTotal;
    QList<QSharedPointer<IElanceJob> > m_jobs;
};
}

#endif // ELANCEJOBSPAGE_H
