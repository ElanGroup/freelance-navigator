#ifndef SEARCHJOBSREQUEST_H
#define SEARCHJOBSREQUEST_H

#include "upworkapirequest.h"

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkSearchJobParameters;

class SearchJobsRequest : public UpworkApiRequest
{
    Q_OBJECT
public:
    explicit SearchJobsRequest(const UpworkSearchJobParameters & searchParameters,
                               const OAuthParameters & authenticationParameters,
                               QNetworkAccessManager * networkManager,
                               QObject * parent = 0);

protected:
    virtual QString path() const override;
    virtual QNetworkAccessManager::Operation operation() const override;
    virtual QUrlQuery query() const override;

private:
    QString m_categoryId;
    QString m_searchQuery;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // SEARCHJOBSREQUEST_H
