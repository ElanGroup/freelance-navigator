#ifndef UPWORKAPICLIENT_H
#define UPWORKAPICLIENT_H

#include <QObject>
#include "upworkenums.h"

namespace FreelanceNavigator
{

class ApiRequest;

namespace Upwork
{

class UpworkSettings;
class RequestFactory;
class UpworkErrorHandler;

class UpworkApiClient : public QObject
{
    Q_OBJECT
public:
    explicit UpworkApiClient(UpworkSettings * settings, QWidget * parent);
    ~UpworkApiClient();
    UpworkApiClient(const UpworkApiClient &) = delete;
    UpworkApiClient & operator=(const UpworkApiClient &) = delete;

    void initialize();
    void loadCategories();

signals:
    void error(UpworkApiError error) const;
    void warning(UpworkApiWarning warning) const;
    void initialized() const;

private slots:
    void processGetRequestTokenResult();
    void processAuthorizationRedirect(const QUrl & url);
    void processGetAccessTokenResult();
    void processLoadCategoriesResult();

private:
    void authorize() const;
    void getAccessToken() const;
    void processError(const UpworkErrorHandler & errorHandler);

    static const QString m_authorizationUrl;

    UpworkSettings * m_settings;
    RequestFactory * m_requestFactory;

    QString m_requestToken;
    QString m_requestTokenSecret;
    QString m_verificationCode;
    QString m_accessToken;
    QString m_accessTokenSecret;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKAPICLIENT_H
