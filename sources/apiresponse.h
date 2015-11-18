#ifndef APIRESPONSE_H
#define APIRESPONSE_H

#include <QNetworkReply>

namespace FreelanceNavigator
{

class ApiResponse
{
public:
    ApiResponse(QNetworkReply::NetworkError networkError, const QByteArray & data);
    virtual ~ApiResponse() {}

    QNetworkReply::NetworkError networkError() const;
    QByteArray data() const;
    virtual bool isSuccess() const = 0;

private:
    QNetworkReply::NetworkError m_networkError;
    QByteArray m_data;
};

} // namespace FreelanceNavigator

#endif // APIRESPONSE_H
