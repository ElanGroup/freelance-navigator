#include <QUrlQuery>
#include "jobsrequest.h"
#include "elancedatareader.h"

using namespace FreelanceNavigator;

const QString JobsRequest::m_jobsUrl("https://api.elance.com/api2/jobs");

JobsRequest::JobsRequest(int category,
                         const QList<int> & subcategories,
                         int page,
                         const QString & accessToken,
                         QNetworkAccessManager * networkManager,
                         QObject * parent)
    : DataRequest(accessToken, networkManager, parent),
      m_category(category),
      m_subcategories(subcategories),
      m_page(page)
{
}

JobsRequest::~JobsRequest()
{
}

QSharedPointer<IElanceJobsPage> JobsRequest::jobsPage() const
{
    return m_jobsPage;
}

void JobsRequest::doSubmit() const
{
    QUrl url(m_jobsUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("access_token", accessToken());
    urlQuery.addQueryItem("rpp", QString::number(m_jobsNumberPerPage));
    urlQuery.addQueryItem("catFilter", QString::number(m_category));
    if (!m_subcategories.isEmpty())
    {
        QStringList subcategoriesList;
        for (int i = 0; i < m_subcategories.count(); ++i)
        {
            subcategoriesList << QString::number(m_subcategories[i]);
        }
        urlQuery.addQueryItem("subcatFilter", subcategoriesList.join(','));
    }
    urlQuery.addQueryItem("page", QString::number(m_page));
    url.setQuery(urlQuery);
    QNetworkRequest request(url);
    QNetworkReply * reply = networkManager()->get(request);
    connect(reply, &QNetworkReply::finished, this, &JobsRequest::processReply);
}

void JobsRequest::readData(const QByteArray & data)
{
    m_jobsPage = ElanceDataReader::readJobsPage(data);
}
