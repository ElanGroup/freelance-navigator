#include <QSettings>
#include "settings.h"

using namespace FreelanceNavigator;

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

void Settings::readUpworkKeys()
{
    QSettings settings(QStringLiteral(":/Resources/upwork-api.ini"), QSettings::IniFormat);
    m_upworkKey = settings.value(QStringLiteral("Key")).toString();
    m_upworkSecret = settings.value(QStringLiteral("Secret")).toString();
}
