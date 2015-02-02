#ifndef ELANCEAPICLIENT_H
#define ELANCEAPICLIENT_H

#include <QObject>
#include <QNetworkRequest>

class QNetworkReply;
class QDialog;
class QNetworkAccessManager;
class QUrlQuery;

namespace FreelanceNavigator
{
class IElanceCategory;
class IElanceJobsPage;
class IElanceError;

class ElanceApiClient : public QObject
{
    Q_OBJECT
public:
    enum ElanceApiError
    {
        UnknownError,
        ConnectionError,
        ServiceError
    };

    explicit ElanceApiClient(QObject * parent = 0);
    ~ElanceApiClient();

    bool readSettings();
    bool authorize();
    void loadCategories();
    void loadJobs(int category, const QList<int> & subcategories, int page);

signals:
    void categoriesLoaded(const QList<QSharedPointer<IElanceCategory> > & categories) const;
    void jobsLoaded(const QSharedPointer<IElanceJobsPage> & jobsPage) const;
    void error(ElanceApiError error) const;

private slots:
    void processAuthorizeReply(QNetworkReply * reply);
    void processTokensReply();
    void processCategoriesReply();
    void processJobsReply();

private:
    void getTokens(const QString & authorizationCode);
    void refreshTokens();
    void loadCategories(const QNetworkRequest & request);
    void loadJobs(const QNetworkRequest & request);
    QNetworkReply * post(const QString & url, const QUrlQuery & data);
    void processPendingRequests();
    void processError(QNetworkReply * reply);
    static bool checkErrorExists(const QList<QSharedPointer<IElanceError> > & errors,
                                 const QString & errorCode);

    static const QString m_authorizeUrl;
    static const QString m_tokenUrl;
    static const QString m_categoriesUrl;
    static const QString m_jobsUrl;

    QString m_clientId;
    QString m_clientSecret;
    QString m_redirectUri;

    QString m_accessToken;
    QString m_refreshToken;
    bool m_isTokensRefreshingActive;
    QList<QNetworkRequest> m_pendingRequests;

    QDialog * m_authorizeDialog;
    QNetworkAccessManager * m_networkManager;

    static const int m_jobsNumberPerPage = 25;
};
}

#endif // ELANCEAPICLIENT_H
