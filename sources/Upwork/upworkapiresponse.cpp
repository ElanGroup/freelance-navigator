#include "upworkapiresponse.h"
#include "upworkerrorhandler.h"

using namespace FreelanceNavigator::Upwork;

UpworkApiResponse::UpworkApiResponse(QNetworkReply::NetworkError networkError,
                                     const QByteArray & data) :
    ApiResponse(networkError, data),
    m_errorHandler(new UpworkErrorHandler())
{
    m_errorHandler->handleError(this);
}

UpworkApiResponse::~UpworkApiResponse()
{
    delete m_errorHandler;
}

bool UpworkApiResponse::isSuccess() const
{
    return networkError() == QNetworkReply::NoError;
}
