#ifndef UPWORKJOBPAGE_H
#define UPWORKJOBPAGE_H

#include <QList>
#include <QSharedPointer>

namespace FreelanceNavigator
{

class Job;

namespace Upwork
{

class UpworkJobPage
{
public:
    UpworkJobPage();
    UpworkJobPage(const UpworkJobPage &) = delete;
    UpworkJobPage & operator=(const UpworkJobPage &) = delete;

    QList<QSharedPointer<FreelanceNavigator::Job>> jobs() const;
    void addJob(const QSharedPointer<FreelanceNavigator::Job> & job);
    int offset() const;
    void setOffset(int offset);
    int count() const;
    void setCount(int count);
    int total() const;
    void setTotal(int total);
    bool isValid() const;

private:
    QList<QSharedPointer<FreelanceNavigator::Job>> m_jobs;
    int m_offset = -1;
    int m_count = -1;
    int m_total = -1;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKJOBPAGE_H
