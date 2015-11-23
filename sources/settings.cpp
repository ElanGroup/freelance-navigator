#include <QSettings>
#include "settings.h"

using namespace FreelanceNavigator;

const QString Settings::m_upworkApiGroupName(QStringLiteral("Upwork API"));
const QString Settings::m_upworkAccessTokenName(QStringLiteral("Access Token"));
const QString Settings::m_upworkAccessTokenSecretName(QStringLiteral("Access Token Secret"));

Settings::Settings()
{
}

QString Settings::upworkKey()
{
    if (m_upworkKey.isEmpty())
    {
        readUpworkKeys();
    }
    return m_upworkKey;
}

QString Settings::upworkSecret()
{
    if (m_upworkSecret.isEmpty())
    {
        readUpworkKeys();
    }
    return m_upworkSecret;
}

QString Settings::upworkAccessToken()
{
    if (m_upworkAccessToken.isEmpty())
    {
        readUpworkAccessToken();
    }
    return m_upworkAccessToken;
}

QString Settings::upworkAccessTokenSecret()
{
    if (m_upworkAccessTokenSecret.isEmpty())
    {
        readUpworkAccessToken();
    }
    return m_upworkAccessTokenSecret;
}

void Settings::saveUpworkAccessToken(const QString & accessToken,
                                     const QString & accessTokenSecret)
{
    QSettings settings;
    settings.beginGroup(m_upworkApiGroupName);
    settings.setValue(m_upworkAccessTokenName, accessToken);
    settings.setValue(m_upworkAccessTokenSecretName, accessTokenSecret);
    m_upworkAccessToken = accessToken;
    m_upworkAccessTokenSecret = accessTokenSecret;
}

void Settings::readUpworkKeys()
{
    QSettings settings(QStringLiteral(":/Resources/upwork-api.ini"), QSettings::IniFormat);
    m_upworkKey = settings.value(QStringLiteral("Key")).toString();
    m_upworkSecret = settings.value(QStringLiteral("Secret")).toString();
}

void Settings::readUpworkAccessToken()
{
    QSettings settings;
    settings.beginGroup(m_upworkApiGroupName);

    QVariant accessToken = settings.value(m_upworkAccessTokenName);
    if (accessToken.isValid())
    {
        m_upworkAccessToken = accessToken.toString();
    }

    QVariant accessTokenSecret = settings.value(m_upworkAccessTokenSecretName);
    if (accessTokenSecret.isValid())
    {
        m_upworkAccessTokenSecret = accessTokenSecret.toString();
    }
}
