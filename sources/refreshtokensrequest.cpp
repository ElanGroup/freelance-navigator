#include "refreshtokensrequest.h"

using namespace FreelanceNavigator;

RefreshTokensRequest::RefreshTokensRequest(const QString & clientId,
                                           const QString & clientSecret,
                                           const QString & refreshToken,
                                           QNetworkAccessManager * networkManager,
                                           QObject * parent)
    : TokensRequest(networkManager, parent)
{
    m_postData.addQueryItem("client_id", clientId);
    m_postData.addQueryItem("client_secret", clientSecret);
    m_postData.addQueryItem("refresh_token", refreshToken);
    m_postData.addQueryItem("grant_type", "refresh_token");
}

RefreshTokensRequest::~RefreshTokensRequest()
{
}

QUrlQuery RefreshTokensRequest::postData() const
{
    return m_postData;
}
