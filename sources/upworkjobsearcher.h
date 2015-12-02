#ifndef UPWORKJOBSEARCHER_H
#define UPWORKJOBSEARCHER_H

#include "jobsearcher.h"
#include "Upwork/upworksearchjobparameters.h"

namespace FreelanceNavigator
{

namespace Upwork
{
class UpworkApiClient;
class UpworkSearchJobParameters;
}

class UpworkJobSearcher : public JobSearcher
{
    Q_OBJECT
public:
    explicit UpworkJobSearcher(Upwork::UpworkSearchJobParameters && parameters,
                               Upwork::UpworkApiClient * upworkApiClient,
                               Widgets::JobListWidget * jobListWidget,
                               QObject * parent = 0);
    UpworkJobSearcher(const UpworkJobSearcher &) = delete;
    UpworkJobSearcher & operator=(const UpworkJobSearcher &) = delete;

    virtual void search() const override;

protected:
    virtual void stopSearch() override;

private slots:
    void processFoundJobs(const QList<QSharedPointer<Job>> & jobs);

private:
    QList<QSharedPointer<Job>> filterJobs(const QList<QSharedPointer<Job>> & jobs) const;
    bool checkPostedDate(const QSharedPointer<Job> & job) const;
    static bool checkDateRange(const QDateTime & date, int days);
    bool checkSearchQuery(const QSharedPointer<Job> & job) const;

    Upwork::UpworkApiClient * m_upworkApiClient;
    Upwork::UpworkSearchJobParameters m_searchParameters;
};

} // namespace FreelanceNavigator

#endif // UPWORKJOBSEARCHER_H
