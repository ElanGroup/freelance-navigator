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

private slots:
    void processFoundJobs(const QList<QSharedPointer<Job>> & jobs);

private:
    Upwork::UpworkApiClient * m_upworkApiClient;
    Upwork::UpworkSearchJobParameters m_searchParameters;
};

} // namespace FreelanceNavigator

#endif // UPWORKJOBSEARCHER_H
