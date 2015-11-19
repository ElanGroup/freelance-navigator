#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QUrlQuery;

namespace FreelanceNavigator
{

class ApiRequest : public QObject
{
    Q_OBJECT
public:
    explicit ApiRequest(QNetworkAccessManager * networkManager, QObject * parent = 0);
    virtual ~ApiRequest();

    virtual void submit() = 0;
    QNetworkReply * reply() const;

signals:
    void finished() const;

protected:
    virtual QUrl url() const = 0;
    QNetworkRequest * request() const;
    void get() const;
    void post(const QUrlQuery & data) const;

private slots:
    void processReply();

private:
    QNetworkAccessManager * m_networkManager;
    QNetworkRequest * m_request;
    QNetworkReply * m_reply;
};

} // namespace FreelanceNavigator

#endif // APIREQUEST_H
