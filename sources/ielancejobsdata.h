#ifndef IELANCEJOBSDATA_H
#define IELANCEJOBSDATA_H

#include <QList>
#include <QSharedPointer>
#include "ielancedata.h"

namespace FreelanceNavigator
{
class IElanceJobData;

class IElanceJobsData : public IElanceData
{
public:
    virtual ~IElanceJobsData() {}
    virtual int jobsTotal() const = 0;
    virtual const QList<QSharedPointer<IElanceJobData> > & jobs() const = 0;
};
}

#endif // IELANCEJOBSDATA_H
