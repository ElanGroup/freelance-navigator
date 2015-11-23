#ifndef OAUTHPARAMETERS_H
#define OAUTHPARAMETERS_H

#include <QString>

namespace FreelanceNavigator
{
namespace Upwork
{

class OAuthParameters
{
public:
    OAuthParameters(const QString & applicationKey, const QString & applicationSecret);
    OAuthParameters(const QString & applicationKey,
                    const QString & applicationSecret,
                    const QString & token,
                    const QString & tokenSecret);
    OAuthParameters(const QString & applicationKey,
                    const QString & applicationSecret,
                    const QString & token,
                    const QString & tokenSecret,
                    const QString & verificationCode);
    OAuthParameters(const OAuthParameters &) = delete;
    OAuthParameters & operator=(const OAuthParameters &) = delete;

    QString applicationKey() const;
    QString applicationSecret() const;
    QString token() const;
    QString tokenSecret() const;
    QString verificationCode() const;

private:
    const QString m_applicationKey;
    const QString m_applicationSecret;
    const QString m_token;
    const QString m_tokenSecret;
    const QString m_verificationCode;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // OAUTHPARAMETERS_H
