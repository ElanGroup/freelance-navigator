#ifndef GETACCESSTOKENREQUEST_H
#define GETACCESSTOKENREQUEST_H

#include "upworkapirequest.h"

namespace FreelanceNavigator
{
namespace Upwork
{

class GetAccessTokenRequest : public UpworkApiRequest
{
    Q_OBJECT
public:
    explicit GetAccessTokenRequest(const OAuthParameters & parameters,
                                   QNetworkAccessManager * networkManager,
                                   QObject * parent = 0);
    GetAccessTokenRequest(const GetAccessTokenRequest &) = delete;
    GetAccessTokenRequest & operator=(const GetAccessTokenRequest &) = delete;

protected:
    virtual QString path() const override;
    virtual QNetworkAccessManager::Operation operation() const override;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // GETACCESSTOKENREQUEST_H
