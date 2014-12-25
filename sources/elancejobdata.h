#ifndef ELANCEJOBDATA_H
#define ELANCEJOBDATA_H

#include "ielancejobdata.h"

namespace FreelanceNavigator
{
class ElanceJobData : public IElanceJobData
{
public:
    ElanceJobData();
    ~ElanceJobData();

    virtual int jobId() const;
    void setJobId(int jobId);

private:
    ElanceJobData(const ElanceJobData &);
    ElanceJobData & operator=(const ElanceJobData &);

    int m_jobId;
};
}

#endif // ELANCEJOBDATA_H
