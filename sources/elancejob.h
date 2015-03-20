#ifndef ELANCEJOB_H
#define ELANCEJOB_H

#include <QDateTime>
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
    virtual const QDateTime & postedDate() const;
    void setPostedDate(const QDateTime & postedDate);
    virtual bool isHourly() const;
    void setIsHourly(bool isHourly);
    virtual QString url() const;
    void setUrl(const QString & url);

private:
    ElanceJob(const ElanceJob &);
    ElanceJob & operator=(const ElanceJob &);

    int m_jobId;
    QString m_name;
    QString m_description;
    QString m_budget;
    QDateTime m_postedDate;
    bool m_isHourly;
    QString m_url;
};
}

#endif // ELANCEJOB_H
