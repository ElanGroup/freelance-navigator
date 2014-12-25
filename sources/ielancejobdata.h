#ifndef IELANCEJOBDATA_H
#define IELANCEJOBDATA_H

namespace FreelanceNavigator
{
class IElanceJobData
{
public:
    virtual ~IElanceJobData() {}
    virtual int jobId() const = 0;
};
}

#endif // IELANCEJOBDATA_H
