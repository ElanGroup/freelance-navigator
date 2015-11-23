#include "oauthparameters.h"

using namespace FreelanceNavigator::Upwork;

OAuthParameters::OAuthParameters(const QString & applicationKey,
                                 const QString & applicationSecret) :
    OAuthParameters(applicationKey, applicationSecret, QString(), QString(), QString())
{
}

OAuthParameters::OAuthParameters(const QString & applicationKey,
                                 const QString & applicationSecret,
                                 const QString & token,
                                 const QString & tokenSecret) :
    OAuthParameters(applicationKey, applicationSecret, token, tokenSecret, QString())
{
}

OAuthParameters::OAuthParameters(const QString & applicationKey,
                                 const QString & applicationSecret,
                                 const QString & token,
                                 const QString & tokenSecret,
                                 const QString & verificationCode) :
    m_applicationKey(applicationKey),
    m_applicationSecret(applicationSecret),
    m_token(token),
    m_tokenSecret(tokenSecret),
    m_verificationCode(verificationCode)
{
}

QString OAuthParameters::applicationKey() const
{
    return m_applicationKey;
}

QString OAuthParameters::applicationSecret() const
{
    return m_applicationSecret;
}

QString OAuthParameters::token() const
{
    return m_token;
}

QString OAuthParameters::tokenSecret() const
{
    return m_tokenSecret;
}

QString OAuthParameters::verificationCode() const
{
    return m_verificationCode;
}
