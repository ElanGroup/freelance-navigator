#ifndef ELANCEJOBSDATA_H
#define ELANCEJOBSDATA_H

#include "ielancejobsdata.h"

namespace FreelanceNavigator
{
class ElanceJobsData : public IElanceJobsData
{
public:
    ElanceJobsData();
    ~ElanceJobsData();

    inline virtual int jobsCount() const { return m_jobsCount; }

    void setJobsCount(int jobsCount);

private:
    ElanceJobsData(const ElanceJobsData &);
    ElanceJobsData & operator=(const ElanceJobsData &);

    int m_jobsCount;
};
}

#endif // ELANCEJOBSDATA_H
