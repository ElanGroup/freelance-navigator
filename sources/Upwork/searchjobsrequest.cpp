#include <QDebug>
#include <QUrlQuery>
#include "searchjobsrequest.h"
#include "upworksearchjobparameters.h"

using namespace FreelanceNavigator::Upwork;

SearchJobsRequest::SearchJobsRequest(const UpworkSearchJobParameters & searchParameters,
                                     const OAuthParameters & authenticationParameters,
                                     QNetworkAccessManager * networkManager,
                                     QObject * parent) :
    UpworkApiRequest(authenticationParameters, networkManager, parent),
    m_category(searchParameters.category()),
    m_subcategories(searchParameters.subcategories().join(',')),
    m_searchQuery(searchParameters.searchQuery()),
    m_postedDateRange(searchParameters.postedDateRange()),
    m_jobType(searchParameters.jobType()),
    m_minBudget(searchParameters.minBudget()),
    m_maxBudget(searchParameters.maxBudget())
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
    urlQuery.addQueryItem(QStringLiteral("category2"), m_category);
    if (!m_subcategories.isEmpty())
    {
        urlQuery.addQueryItem(QStringLiteral("subcategory2"), m_subcategories);
    }
    if (m_postedDateRange != PostedDateRange::Any)
    {
        urlQuery.addQueryItem(QStringLiteral("days_posted"),
                              QString::number(daysPosted(m_postedDateRange)));
    }
    if (m_jobType != JobType::Any)
    {
        urlQuery.addQueryItem(QStringLiteral("job_type"),
                              m_jobType == JobType::Fixed ? QStringLiteral("fixed")
                                                          : QStringLiteral("hourly"));
    }
    if (m_minBudget != -1 || m_maxBudget != -1)
    {
        QString budget;
        if (m_maxBudget == -1)
        {
            budget = QString("%1-").arg(m_minBudget);
        }
        else if (m_minBudget == -1)
        {
            budget = QString("-%1").arg(m_maxBudget);
        }
        else
        {
            budget = QString("%1-%2").arg(m_minBudget).arg(m_maxBudget);
        }
        urlQuery.addQueryItem(QStringLiteral("budget"), budget);
    }
    urlQuery.addQueryItem(QStringLiteral("job_status"), QStringLiteral("open"));
    urlQuery.addQueryItem(QStringLiteral("paging"), QString("%1;%2").arg(m_offset).arg(m_count));
    return urlQuery;
}

int SearchJobsRequest::offset() const
{
    return m_offset;
}

void SearchJobsRequest::setOffset(int offset)
{
    m_offset = offset;
}

int SearchJobsRequest::daysPosted(PostedDateRange postedDateRange)
{
    switch (postedDateRange)
    {
    case PostedDateRange::Day:
        return 1;
    case PostedDateRange::ThreeDays:
        return 3;
    case PostedDateRange::FiveDays:
        return 5;
    case PostedDateRange::Week:
        return 7;
    case PostedDateRange::TenDays:
        return 10;
    case PostedDateRange::TwoWeeks:
        return 14;
    case PostedDateRange::Month:
        return QDateTime::currentDateTime().addMonths(-1).daysTo(QDateTime::currentDateTime());
    default:
        return -1;
    }
}
