#include "gettokensrequest.h"

using namespace FreelanceNavigator;

GetTokensRequest::GetTokensRequest(const QString & authorizationCode,
                                   const QString & clientId,
                                   const QString & clientSecret,
                                   QNetworkAccessManager * networkManager,
                                   QObject * parent)
    : TokensRequest(networkManager, parent)
{
    m_postData.addQueryItem("code", authorizationCode);
    m_postData.addQueryItem("client_id", clientId);
    m_postData.addQueryItem("client_secret", clientSecret);
    m_postData.addQueryItem("grant_type", "authorization_code");
}

GetTokensRequest::~GetTokensRequest()
{
}

QUrlQuery GetTokensRequest::postData() const
{
    return m_postData;
}
