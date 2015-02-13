#ifndef IELANCEJOB_H
#define IELANCEJOB_H

#include <QString>
#include <QSharedPointer>
#include "ielancedata.h"

namespace FreelanceNavigator
{
class IElanceJob : public IElanceData
{
public:
    virtual ~IElanceJob() {}
    virtual int jobId() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString budget() const = 0;
    virtual const QDateTime & postedDate() const = 0;
    virtual bool isHourly() const = 0;
};
}

Q_DECLARE_METATYPE(QSharedPointer<FreelanceNavigator::IElanceJob>)

#endif // IELANCEJOB_H
