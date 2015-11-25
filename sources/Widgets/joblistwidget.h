#ifndef JOBLISTWIDGET_H
#define JOBLISTWIDGET_H

#include <QWidget>

class QStandardItemModel;

namespace Ui
{
class JobListWidget;
}

namespace FreelanceNavigator
{

class Job;

namespace Widgets
{

class JobListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit JobListWidget(QWidget * parent = 0);
    ~JobListWidget();
    JobListWidget(const JobListWidget &) = delete;
    JobListWidget & operator=(const JobListWidget &) = delete;

    void addJobs(const QList<QSharedPointer<Job>> & jobs);

private:
    Ui::JobListWidget * ui;
    QList<QSharedPointer<Job>> m_jobs;
    QStandardItemModel * m_jobModel;
};

} // namespace Widgets
} // namespace FreelanceNavigator

#endif // JOBLISTWIDGET_H
