#include <QNetworkAccessManager>
#include <QUrlQuery>
#include "apirequest.h"
#include "apiresponse.h"

using namespace FreelanceNavigator;

ApiRequest::ApiRequest(QNetworkAccessManager * networkManager, QObject * parent) :
    QObject(parent),
    m_networkManager(networkManager),
    m_request(new QNetworkRequest())
{
}

ApiRequest::~ApiRequest()
{
    delete m_request;
}

QNetworkRequest * ApiRequest::request() const
{
    return m_request;
}

QNetworkReply::NetworkError ApiRequest::error() const
{
    return m_error;
}

QByteArray ApiRequest::replyData() const
{
    return m_replyData;
}

void ApiRequest::get() const
{
    m_request->setUrl(url());
    QNetworkReply * reply = m_networkManager->get(*m_request);
    connect(reply, &QNetworkReply::finished, this, &ApiRequest::processReply);
}

void ApiRequest::post(const QUrlQuery & data) const
{
    m_request->setUrl(url());
    m_request->setHeader(QNetworkRequest::ContentTypeHeader,
                         QStringLiteral("application/x-www-form-urlencoded"));
    QNetworkReply * reply =
        m_networkManager->post(*m_request, data.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, &ApiRequest::processReply);
}

void ApiRequest::processReply()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT(reply);
    m_error = reply->error();
    m_replyData = reply->readAll();
    reply->deleteLater();
    emit finished();
}
