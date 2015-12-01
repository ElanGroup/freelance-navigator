#ifndef JOBSEARCHER_H
#define JOBSEARCHER_H

#include <QObject>

namespace FreelanceNavigator
{

namespace Widgets
{
class JobListWidget;
}

class Job;

class JobSearcher : public QObject
{
    Q_OBJECT
public:
    explicit JobSearcher(Widgets::JobListWidget * jobListWidget, QObject * parent = 0);
    virtual ~JobSearcher();

    virtual void search() const = 0;

signals:
    void searchFinished() const;
    void maxJobCountReached(int count) const;

protected:
    void addJobsToListWidget(const QList<QSharedPointer<Job>> & jobs);
    virtual void stopSearch() = 0;

private:
    Widgets::JobListWidget * m_jobListWidget;
    static const int m_maxJobCount = 1000;
    int m_jobCount = 0;
};

} // namespace FreelanceNavigator

#endif // JOBSEARCHER_H
