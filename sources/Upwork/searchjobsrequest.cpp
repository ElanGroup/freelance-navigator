#include <QUrlQuery>
#include "searchjobsrequest.h"
#include "upworksearchjobparameters.h"

using namespace FreelanceNavigator::Upwork;

SearchJobsRequest::SearchJobsRequest(const UpworkSearchJobParameters & searchParameters,
                                     const OAuthParameters & authenticationParameters,
                                     QNetworkAccessManager * networkManager,
                                     QObject * parent) :
    UpworkApiRequest(authenticationParameters, networkManager, parent),
    m_categoryId(searchParameters.categoryId()),
    m_searchQuery(searchParameters.searchQuery())
{
}

QString SearchJobsRequest::path() const
{
    return QStringLiteral("profiles/v2/search/jobs.json");
}

QNetworkAccessManager::Operation SearchJobsRequest::operation() const
{
    return QNetworkAccessManager::GetOperation;
}

QUrlQuery SearchJobsRequest::query() const
{
    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("q"), m_searchQuery);
    urlQuery.addQueryItem(QStringLiteral("category2"), m_categoryId);
    urlQuery.addQueryItem(QStringLiteral("job_status"), QStringLiteral("open"));
    return urlQuery;
}
