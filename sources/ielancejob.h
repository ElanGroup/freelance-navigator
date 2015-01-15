#ifndef IELANCEJOB_H
#define IELANCEJOB_H

#include "ielancedata.h"

namespace FreelanceNavigator
{
class IElanceJob : public IElanceData
{
public:
    virtual ~IElanceJob() {}
    virtual int jobId() const = 0;
};
}

#endif // IELANCEJOB_H
