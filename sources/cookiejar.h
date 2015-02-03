#ifndef COOKIEJAR_H
#define COOKIEJAR_H

#include <QNetworkCookieJar>

namespace FreelanceNavigator
{
class CookieJar : public QNetworkCookieJar
{
public:
    CookieJar();
    ~CookieJar();

    static void clear();

private:
    void load();
    void save();
    static QString cookiesFilePath();

    static const QString m_cookiesFileName;
};
}

#endif // COOKIEJAR_H
