#include <QUrlQuery>
#include "categoriesrequest.h"
#include "elancedatareader.h"

using namespace FreelanceNavigator;

const QString CategoriesRequest::m_categoriesUrl("https://api.elance.com/api2/categories");

CategoriesRequest::CategoriesRequest(const QString & accessToken,
                                     QNetworkAccessManager * networkManager,
                                     QObject * parent)
    : DataRequest(accessToken, networkManager, parent)
{
}

CategoriesRequest::~CategoriesRequest()
{
}

QList<QSharedPointer<IElanceCategory> > CategoriesRequest::categories() const
{
    return m_categories;
}

void CategoriesRequest::doSubmit() const
{
    QUrl url(m_categoriesUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("access_token", accessToken());
    url.setQuery(urlQuery);
    QNetworkRequest request(url);
    QNetworkReply * reply = networkManager()->get(request);
    connect(reply, &QNetworkReply::finished, this, &CategoriesRequest::processReply);
}

void CategoriesRequest::readData(const QByteArray & data)
{
    m_categories = ElanceDataReader::readCategories(data);
}
