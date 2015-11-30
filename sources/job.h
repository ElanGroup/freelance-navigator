#ifndef JOB_H
#define JOB_H

#include <QString>
#include <QSharedPointer>

namespace FreelanceNavigator
{

enum class JobType
{
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
};

} // namespace FreelanceNavigator

Q_DECLARE_METATYPE(QSharedPointer<FreelanceNavigator::Job>)

#endif // JOB_H
