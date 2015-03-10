#ifndef JOBSMANAGER_H
#define JOBSMANAGER_H

#include <QObject>
#include <QMap>
#include "ielancejob.h"

namespace FreelanceNavigator
{
class JobsManager : public QObject
{
    Q_OBJECT
public:
    explicit JobsManager(QObject * parent);
    ~JobsManager();

    void clear();
    int pagesTotal() const;
    QList<QSharedPointer<IElanceJob> > getJobs(int page) const;

signals:
    void pageAdded();

public slots:
    void processLoadedJobs(QList<QSharedPointer<IElanceJob> > jobs);
    void processLoadFinish();

private:
    Q_DISABLE_COPY(JobsManager)

    void addJobsPage();

    static const int m_pageSize = 25;
    QMap<int, QList<QSharedPointer<IElanceJob> > > m_jobs;
    QList<QSharedPointer<IElanceJob> > m_pageJobs;
};
}

#endif // JOBSMANAGER_H
