#ifndef SEARCHJOBSREQUEST_H
#define SEARCHJOBSREQUEST_H

#include "upworkapirequest.h"
#include "upworkenums.h"
#include "job.h"

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
    static int daysPosted(PostedDateRange postedDateRange);

    QString m_category;
    QString m_subcategories;
    QString m_searchQuery;
    PostedDateRange m_postedDateRange;
    JobType m_jobType;
    int m_minBudget;
    int m_maxBudget;
    int m_offset = 0;
    static const int m_count = 100;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // SEARCHJOBSREQUEST_H
