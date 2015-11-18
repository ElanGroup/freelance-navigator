#ifndef GETREQUESTTOKENREQUEST_H
#define GETREQUESTTOKENREQUEST_H

#include "upworkapirequest.h"

namespace FreelanceNavigator
{
namespace Upwork
{

class GetRequestTokenRequest : public UpworkApiRequest
{
    Q_OBJECT
public:
    explicit GetRequestTokenRequest(const QString & applicationKey,
                                    const QString & applicationSecret,
                                    const QString & callbackUrl,
                                    QNetworkAccessManager * networkManager,
                                    QObject * parent = 0);
    GetRequestTokenRequest(const GetRequestTokenRequest &) = delete;
    GetRequestTokenRequest & operator=(const GetRequestTokenRequest &) = delete;

protected:
    virtual QString relativeUrl() const override;
    virtual QNetworkAccessManager::Operation operation() const override;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // GETREQUESTTOKENREQUEST_H
