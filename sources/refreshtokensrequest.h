#ifndef REFRESHTOKENSREQUEST_H
#define REFRESHTOKENSREQUEST_H

#include <QUrlQuery>
#include "tokensrequest.h"

namespace FreelanceNavigator
{
class RefreshTokensRequest : public TokensRequest
{
    Q_OBJECT
public:
    explicit RefreshTokensRequest(const QString & clientId,
                                  const QString & clientSecret,
                                  const QString & refreshToken,
                                  QNetworkAccessManager * networkManager,
                                  QObject * parent);
    virtual ~RefreshTokensRequest();

protected:
    virtual QUrlQuery postData() const;

private:
    Q_DISABLE_COPY(RefreshTokensRequest)

    QUrlQuery m_postData;
};
}

#endif // REFRESHTOKENSREQUEST_H
