#include <QUrlQuery>
#include <QNetworkReply>
#include "tokensrequest.h"
#include "elancedatareader.h"
#include "ielancetokens.h"

using namespace FreelanceNavigator;

const QString TokensRequest::m_tokenUrl("https://api.elance.com/api2/oauth/token");

TokensRequest::TokensRequest(QNetworkAccessManager * networkManager, QObject * parent)
    : ElanceApiRequest(networkManager, parent)
{
}

TokensRequest::~TokensRequest()
{
}

QString TokensRequest::accessToken() const
{
    return m_accessToken;
}

QString TokensRequest::refreshToken() const
{
    return m_refreshToken;
}

void TokensRequest::doSubmit() const
{
    QNetworkReply * reply = post(m_tokenUrl, postData());
    connect(reply, &QNetworkReply::finished, this, &TokensRequest::processReply);
}

void TokensRequest::processReply()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT(reply);
    setError(reply->error());
    if (reply->error() == QNetworkReply::NoError)
    {
        QSharedPointer<IElanceTokens> tokens = ElanceDataReader::readTokens(reply->readAll());
        if (tokens->isValid())
        {
            m_accessToken = tokens->accessToken();
            m_refreshToken = tokens->refreshToken();
            emit finished(true);
        }
        else
        {
            emit finished(false);
        }
    }
    else
    {
        emit finished(false);
    }
    reply->deleteLater();
}
