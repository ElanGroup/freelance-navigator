#ifndef JOBLISTMODEL_H
#define JOBLISTMODEL_H

#include <QAbstractListModel>

class QPaintDevice;

namespace FreelanceNavigator
{

class Job;

namespace Widgets
{

class JobItem;

class JobListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit JobListModel(QWidget * parent = 0);
    JobListModel(const JobListModel &) = delete;
    JobListModel & operator=(const JobListModel &) = delete;

    virtual int rowCount(const QModelIndex & parent) const override;
    virtual QVariant data(const QModelIndex & index, int role) const override;

    void addJobs(const QList<QSharedPointer<Job>> & jobs);
    void clear();

private:
    QList<QSharedPointer<JobItem>> m_jobList;
    QPaintDevice * m_paintDevice;
};

} // namespace Widgets
} // namespace FreelanceNavigator

#endif // JOBLISTMODEL_H
