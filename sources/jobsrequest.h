#ifndef JOBSREQUEST_H
#define JOBSREQUEST_H

#include "datarequest.h"

namespace FreelanceNavigator
{
class IElanceJobsPage;

class JobsRequest : public DataRequest
{
    Q_OBJECT
public:
    explicit JobsRequest(int category,
                         const QList<int> & subcategories,
                         const QString & keywords,
                         int page,
                         const QString & accessToken,
                         QNetworkAccessManager * networkManager,
                         QObject * parent);
    virtual ~JobsRequest();

    QSharedPointer<IElanceJobsPage> jobsPage() const;

protected:
    virtual void doSubmit() const;
    virtual void readData(const QByteArray & data);

private:
    Q_DISABLE_COPY(JobsRequest)

    int m_category;
    QList<int> m_subcategories;
    QString m_keywords;
    int m_page;
    QSharedPointer<IElanceJobsPage> m_jobsPage;

    static const QString m_jobsUrl;
    static const int m_jobsNumberPerPage = 25;
};
}

#endif // JOBSREQUEST_H
