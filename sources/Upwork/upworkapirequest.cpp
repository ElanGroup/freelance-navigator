#include <QPair>
#include <QUrlQuery>
#include <QMessageAuthenticationCode>
#include "upworkapirequest.h"
#include "upworkapiresponse.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Upwork;

const QString UpworkApiRequest::m_baseUrl("https://www.upwork.com/api/");
const QByteArray UpworkApiRequest::m_callbackParameter("oauth_callback");
const QByteArray UpworkApiRequest::m_consumerKeyParameter("oauth_consumer_key");
const QByteArray UpworkApiRequest::m_nonceParameter("oauth_nonce");
const QByteArray UpworkApiRequest::m_signatureMethodParameter("oauth_signature_method");
const QByteArray UpworkApiRequest::m_timestampParameter("oauth_timestamp");
const QByteArray UpworkApiRequest::m_signatureParameter("oauth_signature");
const QByteArray UpworkApiRequest::m_authorizationHeader("Authorization");
const QByteArray UpworkApiRequest::m_signatureMethod("HMAC-SHA1");

UpworkApiRequest::UpworkApiRequest(const QString & applicationKey,
                                   const QString & applicationSecret,
                                   QNetworkAccessManager * networkManager,
                                   QObject * parent) :
    ApiRequest(networkManager, parent),
    m_applicationKey(applicationKey),
    m_applicationSecret(applicationSecret)
{
}

UpworkApiRequest::~UpworkApiRequest()
{
}

QUrl UpworkApiRequest::url() const
{
    return QUrl(m_baseUrl + relativeUrl());
}

QUrlQuery UpworkApiRequest::postData() const
{
    return QUrlQuery();
}

void UpworkApiRequest::submit()
{
    addAuthorizationHeader();
    switch (operation())
    {
    case QNetworkAccessManager::GetOperation:
        get();
        break;
    case QNetworkAccessManager::PostOperation:
        post(postData());
        break;
    default:
        break;
    }
}

QSharedPointer<ApiResponse> UpworkApiRequest::response() const
{
    return QSharedPointer<ApiResponse>(new UpworkApiResponse(error(), replyData()));
}

void UpworkApiRequest::addAuthorizationHeader()
{
    if (!m_callbackUrl.isEmpty())
    {
        m_oauthParameters.append(qMakePair(m_callbackParameter, m_callbackUrl.toLatin1()));
    }
    m_oauthParameters.append(qMakePair(m_consumerKeyParameter, m_applicationKey.toLatin1()));
    m_oauthParameters.append(qMakePair(m_nonceParameter, nonce()));
    m_oauthParameters.append(qMakePair(m_signatureMethodParameter, m_signatureMethod));
    m_oauthParameters.append(qMakePair(m_timestampParameter, timestamp()));
    m_oauthParameters.append(qMakePair(m_signatureParameter, generateSignature()));
    request()->setRawHeader(m_authorizationHeader, createAuthorizationHeaderValue());
}

void UpworkApiRequest::setCallbackUrl(const QString & callbackUrl)
{
    m_callbackUrl = callbackUrl;
}

void UpworkApiRequest::setTokenSecret(const QString & tokenSecret)
{
    m_tokenSecret = tokenSecret;
}

QByteArray UpworkApiRequest::nonce()
{
    static bool isFirstCall = true;
    if (isFirstCall)
    {
        isFirstCall = false;
        qsrand(QTime::currentTime().msec());
    }
    QString nonce = QString::number(QDateTime::currentDateTimeUtc().toTime_t());
    nonce.append(QString::number(qrand()));
    return nonce.toLatin1();
}

QByteArray UpworkApiRequest::timestamp()
{
    return QString::number(QDateTime::currentDateTimeUtc().toTime_t()).toLatin1();
}

QByteArray UpworkApiRequest::generateSignature() const
{
    QByteArray baseString = getSignatureBaseString();
    QByteArray secret = QUrl::toPercentEncoding(m_applicationSecret) + "&";
    if (!m_tokenSecret.isEmpty())
    {
        secret.append(QUrl::toPercentEncoding(m_tokenSecret));
    }
    return QMessageAuthenticationCode::hash(baseString,
                                            secret,
                                            QCryptographicHash::Sha1).toBase64();
}

QByteArray UpworkApiRequest::getSignatureBaseString() const
{
    QByteArray baseString;
    baseString.append(getOperationName(operation()).toUtf8() + "&");
    baseString.append(QUrl::toPercentEncoding(url().toString(QUrl::RemoveQuery)) + "&");
    baseString.append(getOauthParametersForSignature());
    return baseString;
}

QString UpworkApiRequest::getOperationName(QNetworkAccessManager::Operation operation)
{
    switch (operation )
    {
    case QNetworkAccessManager::GetOperation:
            return "GET";
    case QNetworkAccessManager::PostOperation:
            return "POST";
    case QNetworkAccessManager::PutOperation:
        return "PUT";
    case QNetworkAccessManager::DeleteOperation:
        return "DEL";
    default:
        return "";
    }
}

QByteArray UpworkApiRequest::getOauthParametersForSignature() const
{
    QByteArray parameters;
    for (auto it = m_oauthParameters.constBegin(); it != m_oauthParameters.constEnd(); ++it)
    {
        if (it != m_oauthParameters.constBegin())
        {
            parameters.append("&");
        }
        parameters.append(QUrl::toPercentEncoding(it->first));
        parameters.append("=");
        parameters.append(QUrl::toPercentEncoding(it->second));
    }
    return QUrl::toPercentEncoding(parameters);
}

QByteArray UpworkApiRequest::createAuthorizationHeaderValue() const
{
    QByteArray result("OAuth ");
    for (auto it = m_oauthParameters.constBegin(); it != m_oauthParameters.constEnd(); ++it)
    {
        if (it != m_oauthParameters.constBegin())
        {
            result.append(",");
        }
        result.append(it->first);
        result.append("=\"");
        result.append(QUrl::toPercentEncoding(it->second));
        result.append("\"");
    }
    return result;
}
