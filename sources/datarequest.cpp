#include "datarequest.h"
#include "elancedatareader.h"

using namespace FreelanceNavigator;

DataRequest::DataRequest(const QString & accessToken,
                         QNetworkAccessManager * networkManager,
                         QObject * parent)
    : ElanceApiRequest(networkManager, parent),
      m_accessToken(accessToken)
{
}

DataRequest::~DataRequest()
{
}

QString DataRequest::accessToken() const
{
    return m_accessToken;
}

void DataRequest::setAccessToken(const QString & accessToken)
{
    m_accessToken = accessToken;
}

void DataRequest::processReply()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT(reply);
    setError(reply->error());
    if (reply->error() == QNetworkReply::NoError)
    {
        readData(reply->readAll());
        emit finished(true);
    }
    else
    {
        setErrors(ElanceDataReader::readErrors(reply->readAll()));
        emit finished(false);
    }
    reply->deleteLater();
}
