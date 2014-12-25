#ifndef ELANCEJOBSDATA_H
#define ELANCEJOBSDATA_H

#include "ielancejobsdata.h"

namespace FreelanceNavigator
{
class IElanceJobData;
class ElanceJobData;

class ElanceJobsData : public IElanceJobsData
{
public:
    ElanceJobsData();
    ~ElanceJobsData();

    virtual int jobsTotal() const;
    void setJobsTotal(int jobsTotal);
    virtual const QList<QSharedPointer<IElanceJobData> > & jobs() const;
    void addJob(ElanceJobData * job);

private:
    ElanceJobsData(const ElanceJobsData &);
    ElanceJobsData & operator=(const ElanceJobsData &);

    int m_jobsTotal;
    QList<QSharedPointer<IElanceJobData> > m_jobs;
};
}

#endif // ELANCEJOBSDATA_H
