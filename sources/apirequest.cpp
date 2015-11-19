#include <QDebug>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QNetworkReply>
#include "apirequest.h"

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

QNetworkReply * ApiRequest::reply() const
{
    return m_reply;
}

QNetworkRequest * ApiRequest::request() const
{
    return m_request;
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
    m_reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT(m_reply);
    m_reply->setParent(this);
    emit finished();
}
