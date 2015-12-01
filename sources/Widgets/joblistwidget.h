#ifndef JOBLISTWIDGET_H
#define JOBLISTWIDGET_H

#include <QWidget>

class QListView;

namespace FreelanceNavigator
{

class Job;

namespace Widgets
{

class JobListModel;

class JobListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JobListWidget(QWidget * parent = 0);
    ~JobListWidget();
    JobListWidget(const JobListWidget &) = delete;
    JobListWidget & operator=(const JobListWidget &) = delete;

    void clear();
    void addJobs(const QList<QSharedPointer<Job>> & jobs);

protected:
    virtual void resizeEvent(QResizeEvent * event) override;

private:
    QListView * m_jobListView;
    JobListModel * m_jobListModel;
};

} // namespace Widgets
} // namespace FreelanceNavigator

#endif // JOBLISTWIDGET_H
