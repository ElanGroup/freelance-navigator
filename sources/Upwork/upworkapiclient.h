#ifndef UPWORKAPICLIENT_H
#define UPWORKAPICLIENT_H

#include <QObject>
#include "upworkenums.h"

class QNetworkAccessManager;

namespace FreelanceNavigator
{

class ApiRequest;

namespace Upwork
{

class UpworkSettings;

class UpworkApiClient : public QObject
{
    Q_OBJECT
public:
    explicit UpworkApiClient(UpworkSettings * settings, QWidget * parent);
    UpworkApiClient(const UpworkApiClient &) = delete;
    UpworkApiClient & operator=(const UpworkApiClient &) = delete;

    void initialize();
    void loadCategories();

signals:
    void error(UpworkApiError error) const;
    void initialized() const;

private slots:
    void processGetRequestTokenResult();
    void processAuthorizationRedirect(const QUrl & url);

private:
    void authorize();

    static const QString m_callbackUrl;
    static const QString m_authorizationUrl;

    UpworkSettings * m_settings;
    QNetworkAccessManager * m_networkManager;

    QString m_requestToken;
    QString m_requestTokenSecret;
    QString m_verifier;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKAPICLIENT_H
