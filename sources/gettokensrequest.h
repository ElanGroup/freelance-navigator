#ifndef GETTOKENSREQUEST_H
#define GETTOKENSREQUEST_H

#include <QUrlQuery>
#include "tokensrequest.h"

namespace FreelanceNavigator
{
class GetTokensRequest : public TokensRequest
{
    Q_OBJECT
public:
    explicit GetTokensRequest(const QString & authorizationCode,
                              const QString & clientId,
                              const QString & clientSecret,
                              QNetworkAccessManager * networkManager,
                              QObject * parent);
    virtual ~GetTokensRequest();

protected:
    virtual QUrlQuery postData() const;

private:
    Q_DISABLE_COPY(GetTokensRequest)

    QUrlQuery m_postData;
};
}

#endif // GETTOKENSREQUEST_H
