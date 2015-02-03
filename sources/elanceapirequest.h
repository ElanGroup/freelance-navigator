#ifndef ELANCEAPIREQUEST_H
#define ELANCEAPIREQUEST_H

#include <QObject>
#include <QNetworkReply>

class QNetworkAccessManager;
class QNetworkReply;
class QUrlQuery;

namespace FreelanceNavigator
{
class IElanceError;

class ElanceApiRequest : public QObject
{
    Q_OBJECT
public:
    explicit ElanceApiRequest(QNetworkAccessManager * networkManager, QObject * parent);
    virtual ~ElanceApiRequest();

    void submit();
    QNetworkReply::NetworkError error() const;
    const QList<QSharedPointer<IElanceError> > & errors() const;

signals:
    void finished(bool isOk) const;

protected:
    virtual void doSubmit() const = 0;
    void setError(QNetworkReply::NetworkError error);
    void setErrors(const QList<QSharedPointer<IElanceError> > & errors);
    QNetworkAccessManager * networkManager() const;
    QNetworkReply * post(const QString & url, const QUrlQuery & data) const;

private:
    Q_DISABLE_COPY(ElanceApiRequest)

    QNetworkAccessManager * m_networkManager;
    QNetworkReply::NetworkError m_error;
    QList<QSharedPointer<IElanceError> > m_errors;
};
}

#endif // ELANCEAPIREQUEST_H
