#ifndef UPWORKAPICLIENT_H
#define UPWORKAPICLIENT_H

#include <QObject>
#include "upworkenums.h"

namespace FreelanceNavigator
{

class ApiRequest;
class Job;

namespace Upwork
{

class UpworkSettings;
class RequestFactory;
class UpworkErrorHandler;
class UpworkCategory;
class UpworkSearchJobParameters;

class UpworkApiClient : public QObject
{
    Q_OBJECT
public:
    explicit UpworkApiClient(UpworkSettings * settings, QWidget * parent);
    ~UpworkApiClient();
    UpworkApiClient(const UpworkApiClient &) = delete;
    UpworkApiClient & operator=(const UpworkApiClient &) = delete;

    bool isLoggedIn() const;
    void initialize();
    void loadCategories();
    void searchJobs(const UpworkSearchJobParameters & parameters);
    void stopSearchJobs();
    void logOut();

signals:
    void error(UpworkApiError error) const;
    void warning(UpworkApiWarning warning) const;
    void initialized() const;
    void categoriesLoaded(const QList<QSharedPointer<UpworkCategory>> & categories) const;
    void jobsLoaded(const QList<QSharedPointer<Job>> & jobs) const;
    void jobSearchingFinished() const;

private slots:
    void processGetRequestTokenResult();
    void processAuthorizationRedirect(const QUrl & url);
    void processGetAccessTokenResult();
    void processLoadCategoriesResult();
    void processSearchJobsResult();

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

    bool m_stopJobSearching = false;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKAPICLIENT_H
