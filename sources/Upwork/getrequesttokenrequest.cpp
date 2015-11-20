#include <QUrlQuery>
#include "getrequesttokenrequest.h"

using namespace FreelanceNavigator::Upwork;

GetRequestTokenRequest::GetRequestTokenRequest(const QString & applicationKey,
                                               const QString & applicationSecret,
                                               QNetworkAccessManager * networkManager,
                                               QObject * parent) :
    UpworkApiRequest(applicationKey, applicationSecret, networkManager, parent)
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
