#ifndef UPWORKJOB_H
#define UPWORKJOB_H

#include "job.h"

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkJob : public Job
{
public:
    UpworkJob();
    UpworkJob(const UpworkJob &) = delete;
    UpworkJob & operator=(const UpworkJob &) = delete;

    virtual QString jobId() const override;
    void setJobId(const QString & jobId);
    virtual QString title() const override;
    void setTitle(const QString & title);
    virtual QString description() const override;
    void setDescription(const QString & description);
    virtual JobType type() const override;
    void setType(JobType type);
    virtual int budget() const override;
    void setBudget(int budget);

private:
    QString m_jobId;
    QString m_title;
    QString m_description;
    JobType m_type;
    int m_budget = -1;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKJOB_H
