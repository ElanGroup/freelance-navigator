#include <QDebug>
#include <QNetworkReply>
#include "upworkerrorhandler.h"

using namespace FreelanceNavigator::Upwork;

UpworkErrorHandler::UpworkErrorHandler(QNetworkReply * reply) : m_reply(reply)
{
}

bool UpworkErrorHandler::hasError() const
{
    return m_reply->error() != QNetworkReply::NoError;
}

bool UpworkErrorHandler::isConnectionError() const
{
    QNetworkReply::NetworkError error = m_reply->error();
    return error == QNetworkReply::ConnectionRefusedError ||
           error == QNetworkReply::RemoteHostClosedError ||
           error == QNetworkReply::HostNotFoundError ||
           error == QNetworkReply::TimeoutError ||
           error == QNetworkReply::SslHandshakeFailedError ||
           error == QNetworkReply::TemporaryNetworkFailureError ||
           error == QNetworkReply::NetworkSessionFailedError ||
           error == QNetworkReply::BackgroundRequestNotAllowedError ||
           error == QNetworkReply::ProxyConnectionRefusedError ||
           error == QNetworkReply::ProxyConnectionClosedError ||
           error == QNetworkReply::ProxyNotFoundError ||
           error == QNetworkReply::ProxyTimeoutError ||
           error == QNetworkReply::UnknownNetworkError ||
           error == QNetworkReply::UnknownProxyError;
}
