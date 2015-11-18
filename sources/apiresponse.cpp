#include "apiresponse.h"

using namespace FreelanceNavigator;

ApiResponse::ApiResponse(QNetworkReply::NetworkError networkError, const QByteArray & data) :
    m_networkError(networkError),
    m_data(data)
{
}

QNetworkReply::NetworkError ApiResponse::networkError() const
{
    return m_networkError;
}

QByteArray ApiResponse::data() const
{
    return m_data;
}
