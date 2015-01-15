#ifndef IELANCEJOBSPAGE_H
#define IELANCEJOBSPAGE_H

#include <QList>
#include <QSharedPointer>
#include "ielancedata.h"

namespace FreelanceNavigator
{
class IElanceJob;

class IElanceJobsPage : public IElanceData
{
public:
    virtual ~IElanceJobsPage() {}
    virtual int jobsTotal() const = 0;
    virtual const QList<QSharedPointer<IElanceJob> > & jobs() const = 0;
};
}

#endif // IELANCEJOBSPAGE_H
