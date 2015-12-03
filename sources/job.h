#ifndef JOB_H
#define JOB_H

#include <QString>
#include <QSharedPointer>

namespace FreelanceNavigator
{

enum class JobType
{
    Any,
    Fixed,
    Hourly
};

class Job
{
public:
    virtual ~Job() {}
    virtual QString jobId() const = 0;
    virtual QString title() const = 0;
    virtual QString description() const = 0;
    virtual JobType type() const = 0;
    virtual int budget() const = 0;
    virtual const QDateTime & postedDate() const = 0;
    virtual QStringList skills() const = 0;
    virtual QString url() const = 0;
};

} // namespace FreelanceNavigator

#endif // JOB_H
