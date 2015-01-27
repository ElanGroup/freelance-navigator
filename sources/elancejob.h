#ifndef ELANCEJOB_H
#define ELANCEJOB_H

#include "ielancejob.h"

namespace FreelanceNavigator
{
class ElanceJob : public IElanceJob
{
public:
    ElanceJob();
    ~ElanceJob();

    virtual bool isValid() const;
    virtual int jobId() const;
    void setJobId(int jobId);
    virtual QString name() const;
    void setName(const QString & name);
    virtual QString description() const;
    void setDescription(const QString & description);
    virtual QString budget() const;
    void setBudget(const QString & budget);
    virtual bool isHourly() const;
    void setIsHourly(bool isHourly);

private:
    ElanceJob(const ElanceJob &);
    ElanceJob & operator=(const ElanceJob &);

    int m_jobId;
    QString m_name;
    QString m_description;
    QString m_budget;
    bool m_isHourly;
};
}

#endif // ELANCEJOB_H
