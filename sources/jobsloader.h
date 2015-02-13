#ifndef JOBSLOADER_H
#define JOBSLOADER_H

#include <QObject>
#include "ielancejob.h"
#include "elanceapiclient.h"

namespace FreelanceNavigator
{
class IElanceJobsPage;

class JobsLoader : public QObject
{
    Q_OBJECT
public:
    enum JobType
    {
        Any,
        FixedPrice,
        Hourly
    };

    explicit JobsLoader(ElanceApiClient * elanceApiClient, QObject * parent);
    ~JobsLoader();

    void setCategory(int category);
    void setSubcategories(const QList<int> & subcategories);
    void setJobType(JobType jobType);
    void load(int page);
    const QList<QSharedPointer<IElanceJob> > & jobs() const;
    int currentPage() const;
    bool areMoreJobsAvailable() const;

signals:
    void loaded(bool isOk) const;

private slots:
    void processLoadedJobs(const QSharedPointer<IElanceJobsPage> & jobsPage);
    void processLoadError(ElanceApiClient::ElanceApiError) const;

private:
    Q_DISABLE_COPY(JobsLoader)

    bool checkJob(const QSharedPointer<IElanceJob> & job) const;

    ElanceApiClient * m_elanceApiClient;
    int m_category;
    QList<int> m_subcategories;
    JobType m_jobType;
    int m_requestedPage;
    int m_currentPage;    
    QList<QSharedPointer<IElanceJob> > m_jobs;
    bool m_areMoreJobsAvailable;

    static const int m_pageSize = 25;
};
}

#endif // JOBSLOADER_H
