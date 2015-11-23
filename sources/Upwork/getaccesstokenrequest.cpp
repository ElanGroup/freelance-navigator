#include "getaccesstokenrequest.h"

using namespace FreelanceNavigator::Upwork;

GetAccessTokenRequest::GetAccessTokenRequest(const OAuthParameters & parameters,
                                             QNetworkAccessManager * networkManager,
                                             QObject * parent) :
    UpworkApiRequest(parameters, networkManager, parent)
{
}

QString GetAccessTokenRequest::path() const
{
    return QStringLiteral("auth/v1/oauth/token/access");
}

QNetworkAccessManager::Operation GetAccessTokenRequest::operation() const
{
    return QNetworkAccessManager::PostOperation;
}
