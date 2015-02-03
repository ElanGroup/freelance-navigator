#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include "elanceapirequest.h"

using namespace FreelanceNavigator;

ElanceApiRequest::ElanceApiRequest(QNetworkAccessManager * networkManager, QObject * parent)
    : QObject(parent),
      m_networkManager(networkManager)
{
}

ElanceApiRequest::~ElanceApiRequest()
{
}

void ElanceApiRequest::submit()
{
    m_error = QNetworkReply::NoError;
    m_errors.clear();
    doSubmit();
}

QNetworkReply::NetworkError ElanceApiRequest::error() const
{
    return m_error;
}

void ElanceApiRequest::setError(QNetworkReply::NetworkError error)
{
    m_error = error;
}

const QList<QSharedPointer<IElanceError> > & ElanceApiRequest::errors() const
{
    return m_errors;
}

void ElanceApiRequest::setErrors(const QList<QSharedPointer<IElanceError> > & errors)
{
    m_errors = errors;
}

QNetworkAccessManager * ElanceApiRequest::networkManager() const
{
    return m_networkManager;
}

QNetworkReply * ElanceApiRequest::post(const QString & url, const QUrlQuery & data) const
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    return m_networkManager->post(request, data.toString(QUrl::FullyEncoded).toUtf8());
}
