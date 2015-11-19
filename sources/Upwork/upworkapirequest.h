#ifndef UPWORKAPIREQUEST_H
#define UPWORKAPIREQUEST_H

#include "apirequest.h"
#include <QNetworkAccessManager>

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkApiRequest : public ApiRequest
{
    Q_OBJECT
public:
    explicit UpworkApiRequest(const QString & applicationKey,
                              const QString & applicationSecret,
                              QNetworkAccessManager * networkManager,
                              QObject * parent = 0);
    virtual ~UpworkApiRequest();

    virtual void submit() override;

protected:
    void setCallbackUrl(const QString & callbackUrl);
    void setTokenSecret(const QString & tokenSecret);
    virtual QUrl url() const override;
    virtual QString path() const = 0;
    virtual QNetworkAccessManager::Operation operation() const = 0;
    virtual QUrlQuery postData() const;
    virtual QUrlQuery query() const;

private:
    QString basicUrl() const;
    void addAuthorizationHeader();
    static QByteArray nonce();
    static QByteArray timestamp();
    QByteArray generateSignature() const;
    QByteArray getSignatureBaseString() const;
    static QString getOperationName(QNetworkAccessManager::Operation operation);
    QByteArray getOauthParametersForSignature() const;
    QByteArray createAuthorizationHeaderValue() const;

    static const QString m_baseUrl;

    static const QByteArray m_callbackParameter;
    static const QByteArray m_consumerKeyParameter;
    static const QByteArray m_nonceParameter;
    static const QByteArray m_signatureMethodParameter;
    static const QByteArray m_timestampParameter;
    static const QByteArray m_signatureParameter;
    static const QByteArray m_authorizationHeader;

    static const QByteArray m_signatureMethod;

    QString m_applicationKey;
    QString m_applicationSecret;
    QString m_callbackUrl;
    QString m_tokenSecret;
    QList<QPair<QByteArray, QByteArray> > m_oauthParameters;
};

} // namespace FreelanceNavigator
} // namespace Upwork

#endif // UPWORKAPIREQUEST_H
