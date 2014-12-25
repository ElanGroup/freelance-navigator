#ifndef IELANCEJOBSDATA_H
#define IELANCEJOBSDATA_H

#include "ielancedata.h"

namespace FreelanceNavigator
{
class IElanceJobsData : public IElanceData
{
public:
    virtual ~IElanceJobsData() {}
    virtual int jobsCount() const = 0;
};
}

#endif // IELANCEJOBSDATA_H
