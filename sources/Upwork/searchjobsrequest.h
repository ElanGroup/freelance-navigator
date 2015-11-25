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
    SearchJobsRequest(const SearchJobsRequest &) = delete;
    SearchJobsRequest & operator=(const SearchJobsRequest &) = delete;

    int offset() const;
    void setOffset(int offset);

protected:
    virtual QString path() const override;
    virtual QNetworkAccessManager::Operation operation() const override;
    virtual QUrlQuery query() const override;

private:
    QString m_categoryId;
    QString m_searchQuery;
    int m_offset = 0;
    static const int m_count = 100;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // SEARCHJOBSREQUEST_H
