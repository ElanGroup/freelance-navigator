#include <QUrlQuery>
#include "getrequesttokenrequest.h"

using namespace FreelanceNavigator::Upwork;

GetRequestTokenRequest::GetRequestTokenRequest(const OAuthParameters & parameters,
                                               QNetworkAccessManager * networkManager,
                                               QObject * parent) :
    UpworkApiRequest(parameters, networkManager, parent)
{
}

QString GetRequestTokenRequest::path() const
{
    return QStringLiteral("auth/v1/oauth/token/request");
}

QNetworkAccessManager::Operation GetRequestTokenRequest::operation() const
{
    return QNetworkAccessManager::PostOperation;
}
