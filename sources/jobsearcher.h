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

protected:
    void addJobsToListWidget(const QList<QSharedPointer<Job>> & jobs);

private:
    Widgets::JobListWidget * m_jobListWidget;
};

} // namespace FreelanceNavigator

#endif // JOBSEARCHER_H
