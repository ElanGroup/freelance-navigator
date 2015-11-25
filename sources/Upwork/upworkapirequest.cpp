#include <QDebug>
#include <QPair>
#include <QUrlQuery>
#include <QMessageAuthenticationCode>
#include <QNetworkRequest>
#include "upworkapirequest.h"
#include "oauthparameters.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Upwork;

const QString UpworkApiRequest::m_baseUrl("https://www.upwork.com/api/");
const QByteArray UpworkApiRequest::m_consumerKeyParameter("oauth_consumer_key");
const QByteArray UpworkApiRequest::m_nonceParameter("oauth_nonce");
const QByteArray UpworkApiRequest::m_signatureMethodParameter("oauth_signature_method");
const QByteArray UpworkApiRequest::m_timestampParameter("oauth_timestamp");
const QByteArray UpworkApiRequest::m_tokenParameter("oauth_token");
const QByteArray UpworkApiRequest::m_verifierParameter("oauth_verifier");
const QByteArray UpworkApiRequest::m_signatureParameter("oauth_signature");
const QByteArray UpworkApiRequest::m_authorizationHeader("Authorization");
const QByteArray UpworkApiRequest::m_signatureMethod("HMAC-SHA1");

UpworkApiRequest::UpworkApiRequest(const OAuthParameters & parameters,
                                   QNetworkAccessManager * networkManager,
                                   QObject * parent) :
    ApiRequest(networkManager, parent),
    m_applicationKey(parameters.applicationKey()),
    m_applicationSecret(parameters.applicationSecret()),
    m_token(parameters.token()),
    m_tokenSecret(parameters.tokenSecret()),
    m_verificationCode(parameters.verificationCode())
{
}

UpworkApiRequest::~UpworkApiRequest()
{
}

QUrl UpworkApiRequest::url() const
{
    QUrl url(basicUrl());
    url.setQuery(query());
    return url;
}

QUrlQuery UpworkApiRequest::postData() const
{
    return QUrlQuery();
}

QUrlQuery UpworkApiRequest::query() const
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

QString UpworkApiRequest::basicUrl() const
{
    return m_baseUrl + path();
}

void UpworkApiRequest::addAuthorizationHeader()
{
    m_oauthParameters.append(qMakePair(m_consumerKeyParameter, m_applicationKey.toLatin1()));
    m_oauthParameters.append(qMakePair(m_nonceParameter, nonce()));
    m_oauthParameters.append(qMakePair(m_signatureMethodParameter, m_signatureMethod));
    m_oauthParameters.append(qMakePair(m_timestampParameter, timestamp()));
    if (!m_token.isEmpty())
    {
        m_oauthParameters.append(qMakePair(m_tokenParameter, m_token.toLatin1()));
    }
    if (!m_verificationCode.isEmpty())
    {
        m_oauthParameters.append(qMakePair(m_verifierParameter, m_verificationCode.toLatin1()));
    }
    request()->setRawHeader(m_authorizationHeader, createAuthorizationHeaderValue());
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
    baseString.append(QUrl::toPercentEncoding(basicUrl()) + "&");
    baseString.append(getParametersForSignature());
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

QByteArray UpworkApiRequest::getParametersForSignature() const
{
    QMap<QByteArray, QByteArray> parametersMap;
    for (auto it = m_oauthParameters.constBegin(); it != m_oauthParameters.constEnd(); ++it)
    {
        parametersMap.insert(it->first, it->second);
    }
    auto queryItems = query().queryItems();
    for (auto it = queryItems.constBegin(); it != queryItems.constEnd(); ++it)
    {
        parametersMap.insert(it->first.toUtf8(), it->second.toUtf8());
    }
    auto postItems = postData().queryItems();
    for (auto it = postItems.constBegin(); it != postItems.constEnd(); ++it)
    {
        parametersMap.insert(it->first.toUtf8(), it->second.toUtf8());
    }

    QByteArray parameters;
    for (auto it = parametersMap.constBegin(); it != parametersMap.constEnd(); ++it)
    {
        if (it != parametersMap.constBegin())
        {
            parameters.append("&");
        }
        parameters.append(QUrl::toPercentEncoding(it.key()));
        parameters.append("=");
        parameters.append(QUrl::toPercentEncoding(it.value()));
    }
    return QUrl::toPercentEncoding(parameters);
}

QByteArray UpworkApiRequest::createAuthorizationHeaderValue() const
{
    QByteArray result("OAuth ");
    for (auto it = m_oauthParameters.constBegin(); it != m_oauthParameters.constEnd(); ++it)
    {
        result.append(it->first);
        result.append("=\"");
        result.append(QUrl::toPercentEncoding(it->second));
        result.append("\",");
    }
    result.append(m_signatureParameter);
    result.append("=\"" + QUrl::toPercentEncoding(generateSignature()) + "\"");
    return result;
}
