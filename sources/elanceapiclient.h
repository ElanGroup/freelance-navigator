#ifndef ELANCEAPICLIENT_H
#define ELANCEAPICLIENT_H

#include <QObject>

class QNetworkReply;
class QDialog;
class QNetworkAccessManager;
class QUrlQuery;

namespace FreelanceNavigator
{
class IElanceCategory;
class IElanceJobsPage;

class ElanceApiClient : public QObject
{
    Q_OBJECT
public:
    explicit ElanceApiClient(QObject * parent = 0);
    ~ElanceApiClient();

    bool readSettings();
    bool authorize();
    void loadCategories();
    void loadJobs();

signals:
    void categoriesLoaded(const QList<QSharedPointer<IElanceCategory> > & categories) const;
    void jobsLoaded(const QSharedPointer<IElanceJobsPage> & jobsPage) const;

private slots:
    void processAuthorizeReply(QNetworkReply * reply);
    void processTokensReply();
    void processCategoriesReply();
    void processJobsReply();

private:
    void getTokens(const QString & authorizationCode);
    QNetworkReply * post(const QString & url, const QUrlQuery & data);

    static const QString m_authorizeUrl;
    static const QString m_tokenUrl;
    static const QString m_categoriesUrl;
    static const QString m_jobsUrl;

    QString m_clientId;
    QString m_clientSecret;
    QString m_redirectUri;

    QString m_accessToken;
    QString m_refreshToken;

    QDialog * m_authorizeDialog;
    QNetworkAccessManager * m_networkManager;

    static const int m_jobsNumberPerPage = 25;
};
}

#endif // ELANCEAPICLIENT_H
