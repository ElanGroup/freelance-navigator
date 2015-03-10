#ifndef JOBSLOADER_H
#define JOBSLOADER_H

#include <QObject>
#include "ielancejob.h"
#include "elanceapiclient.h"
#include "enums.h"

namespace FreelanceNavigator
{
class IElanceJobsPage;

class JobsLoader : public QObject
{
    Q_OBJECT
public:
    explicit JobsLoader(ElanceApiClient * elanceApiClient, QObject * parent);
    ~JobsLoader();

    void setCategory(int category);
    void setSubcategories(const QList<int> & subcategories);
    void setJobType(JobType::Enum jobType);
    void setPostedDateRange(PostedDateRange::Enum postedDateRange);
    void load();

signals:
    void loaded(QList<QSharedPointer<IElanceJob> > jobs) const;
    void loadFinished() const;

private slots:
    void processLoadedJobs(const QSharedPointer<IElanceJobsPage> & jobsPage);
    void processLoadError(ElanceApiClient::ElanceApiError) const;

private:
    Q_DISABLE_COPY(JobsLoader)

    bool checkJob(const QSharedPointer<IElanceJob> & job) const;
    bool checkJobType(const QSharedPointer<IElanceJob> & job) const;
    bool checkPostedDate(const QSharedPointer<IElanceJob> & job) const;
    static bool checkIsInRange(const QDateTime & dateTime, int days);

    ElanceApiClient * m_elanceApiClient;
    int m_category;
    QList<int> m_subcategories;
    JobType::Enum m_jobType;
    PostedDateRange::Enum m_postedDateRange;

    static const int m_dayLength = 24 * 60 * 60; // seconds
};
}

#endif // JOBSLOADER_H
