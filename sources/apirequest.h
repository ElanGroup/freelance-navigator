#ifndef APIREQUEST_H
#define APIREQUEST_H

#include <QObject>
#include <QNetworkReply>

class QNetworkAccessManager;

namespace FreelanceNavigator
{

class ApiResponse;

class ApiRequest : public QObject
{
    Q_OBJECT
public:
    explicit ApiRequest(QNetworkAccessManager * networkManager, QObject * parent = 0);
    virtual ~ApiRequest();

    virtual void submit() = 0;
    virtual QSharedPointer<ApiResponse> response() const = 0;

signals:
    void finished() const;

protected:
    virtual QUrl url() const = 0;
    QNetworkRequest * request() const;
    QNetworkReply::NetworkError error() const;
    QByteArray replyData() const;
    void get() const;
    void post(const QUrlQuery & data) const;

private slots:
    void processReply();

private:
    QNetworkAccessManager * m_networkManager;
    QNetworkRequest * m_request;
    QNetworkReply::NetworkError m_error;
    QByteArray m_replyData;
};

} // namespace FreelanceNavigator

#endif // APIREQUEST_H
