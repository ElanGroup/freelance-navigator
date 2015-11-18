#ifndef UPWORKAPIRESPONSE_H
#define UPWORKAPIRESPONSE_H

#include "apiresponse.h"

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkErrorHandler;

class UpworkApiResponse : public ApiResponse
{
public:
    UpworkApiResponse(QNetworkReply::NetworkError networkError, const QByteArray & data);
    ~UpworkApiResponse();
    UpworkApiResponse(const UpworkApiResponse &) = delete;
    UpworkApiResponse & operator=(const UpworkApiResponse &) = delete;

    virtual bool isSuccess() const override;

private:
    UpworkErrorHandler * m_errorHandler;
};

} // namespace FreelanceNavigator
} // namespace Upwork

#endif // UPWORKAPIRESPONSE_H
