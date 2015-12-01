#ifndef JOBLISTMODEL_H
#define JOBLISTMODEL_H

#include <QAbstractListModel>

namespace FreelanceNavigator
{

class Job;

namespace Widgets
{

class JobListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit JobListModel(QObject * parent = 0);
    JobListModel(const JobListModel &) = delete;
    JobListModel & operator=(const JobListModel &) = delete;

    virtual int rowCount(const QModelIndex & parent) const override;
    virtual QVariant data(const QModelIndex & index, int role) const override;

    void addJobs(const QList<QSharedPointer<Job>> & jobs);
    void clear();

protected:
    virtual bool canFetchMore(const QModelIndex & parent) const override;
    virtual void fetchMore(const QModelIndex & parent) override;

private:
    QList<QSharedPointer<Job>> m_jobList;
    int m_jobCount = 0;
};

} // namespace Widgets
} // namespace FreelanceNavigator

#endif // JOBLISTMODEL_H
