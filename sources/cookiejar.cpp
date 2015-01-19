#include <QSettings>
#include <QDir>
#include <QNetworkCookie>
#include <QStandardPaths>
#include <QDateTime>
#include "cookiejar.h"

using namespace FreelanceNavigator;

const QString CookieJar::m_cookiesFileName("cookies.ini");

CookieJar::CookieJar()
{
    load();
}

CookieJar::~CookieJar()
{
    save();
}

void CookieJar::load()
{
    QSettings cookieSettings(cookiesFilePath(), QSettings::IniFormat);
    QVariant cookiesValue = cookieSettings.value("cookies");
    if (cookiesValue.isValid())
    {
        QList<QByteArray> cookiesData = qvariant_cast<QList<QByteArray> >(cookiesValue);
        foreach (const QByteArray & cookieData, cookiesData)
        {
            foreach (const QNetworkCookie & cookie, QNetworkCookie::parseCookies(cookieData))
            {
                if (cookie.expirationDate().isValid() &&
                    cookie.expirationDate() < QDateTime::currentDateTime())
                {
                    continue;
                }
                insertCookie(cookie);
            }
        }
    }
}

void CookieJar::save()
{
    QList<QByteArray> cookiesData;
    foreach (const QNetworkCookie & cookie, allCookies())
    {
        if (!cookie.isSessionCookie())
        {
            cookiesData.append(cookie.toRawForm());
        }
    }
    QSettings cookieSettings(cookiesFilePath(), QSettings::IniFormat);
    cookieSettings.setValue("cookies", QVariant::fromValue<QList<QByteArray> >(cookiesData));
}

QString CookieJar::cookiesFilePath()
{
    QDir directory(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    return directory.filePath(m_cookiesFileName);
}
