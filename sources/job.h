#ifndef JOB_H
#define JOB_H

#include <QString>
#include <QSharedPointer>

namespace FreelanceNavigator
{

class Job
{
public:
    virtual ~Job() {}
    virtual QString jobId() const = 0;
    virtual QString title() const = 0;
    virtual QString description() const = 0;
};

} // namespace FreelanceNavigator

Q_DECLARE_METATYPE(QSharedPointer<FreelanceNavigator::Job>)

#endif // JOB_H
