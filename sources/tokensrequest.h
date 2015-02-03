#ifndef TOKENSREQUEST_H
#define TOKENSREQUEST_H

#include "elanceapirequest.h"

namespace FreelanceNavigator
{
class TokensRequest : public ElanceApiRequest
{
    Q_OBJECT
public:
    explicit TokensRequest(QNetworkAccessManager * networkManager, QObject * parent);
    virtual ~TokensRequest();

    QString accessToken() const;
    QString refreshToken() const;

protected:
    virtual void doSubmit() const;
    virtual QUrlQuery postData() const = 0;

private slots:
    void processReply();

private:
    Q_DISABLE_COPY(TokensRequest)

    QString m_accessToken;
    QString m_refreshToken;

    static const QString m_tokenUrl;
};
}

#endif // TOKENSREQUEST_H
