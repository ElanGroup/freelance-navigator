#ifndef ELANCEJOB_H
#define ELANCEJOB_H

#include "ielancejob.h"

namespace FreelanceNavigator
{
class ElanceJob : public IElanceJob
{
public:
    ElanceJob();
    ~ElanceJob();

    virtual bool isValid() const;
    virtual int jobId() const;
    void setJobId(int jobId);

private:
    ElanceJob(const ElanceJob &);
    ElanceJob & operator=(const ElanceJob &);

    int m_jobId;
};
}

#endif // ELANCEJOB_H
