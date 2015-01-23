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

private:
    ElanceJob(const ElanceJob &);
    ElanceJob & operator=(const ElanceJob &);

    int m_jobId;
    QString m_name;
    QString m_description;
};
}

#endif // ELANCEJOB_H
