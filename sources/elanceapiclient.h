#ifndef ELANCEAPICLIENT_H
#define ELANCEAPICLIENT_H

#include <QObject>
#include <QNetworkRequest>

class QNetworkReply;
class QDialog;
class QNetworkAccessManager;

namespace FreelanceNavigator
{
class IElanceCategory;
class IElanceJobsPage;
class IElanceError;
class ElanceApiRequest;
class DataRequest;

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
    void processGetTokensResult(bool isOk);
    void processRefreshTokensResult(bool isOk);
    void processCategoriesResult(bool isOk);
    void processJobsResult(bool isOk);

private:
    void getTokens(const QString & authorizationCode);
    void refreshTokens();
    void saveTokens() const;
    void processPendingRequests();
    void processError(DataRequest * request);
    static bool checkErrorExists(const QList<QSharedPointer<IElanceError> > & errors,
                                 const QString & errorCode);

    static const QString m_authorizeUrl;

    QString m_clientId;
    QString m_clientSecret;
    QString m_redirectUri;

    QString m_accessToken;
    QString m_refreshToken;
    bool m_isTokensRefreshingActive;
    QList<DataRequest *> m_pendingRequests;

    QDialog * m_authorizeDialog;
    QNetworkAccessManager * m_networkManager;
};
}

#endif // ELANCEAPICLIENT_H
