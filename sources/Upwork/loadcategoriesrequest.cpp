#include "loadcategoriesrequest.h"

using namespace FreelanceNavigator::Upwork;

LoadCategoriesRequest::LoadCategoriesRequest(const OAuthParameters & parameters,
                                             QNetworkAccessManager * networkManager,
                                             QObject * parent) :
    UpworkApiRequest(parameters, networkManager, parent)
{
}

QString LoadCategoriesRequest::path() const
{
    return QStringLiteral("profiles/v2/metadata/categories.json");
}

QNetworkAccessManager::Operation LoadCategoriesRequest::operation() const
{
    return QNetworkAccessManager::GetOperation;
}
