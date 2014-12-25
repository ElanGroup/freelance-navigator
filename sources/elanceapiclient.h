#ifndef ELANCEAPICLIENT_H
#define ELANCEAPICLIENT_H

#include <QObject>

class QNetworkReply;
class QDialog;
class QNetworkAccessManager;
class QUrlQuery;

namespace FreelanceNavigator
{
class IElanceJobsData;

class ElanceApiClient : public QObject
{
    Q_OBJECT
public:
    explicit ElanceApiClient(QObject * parent = 0);
    ~ElanceApiClient();

    bool readSettings();
    bool authorize();
    void loadJobs();

signals:
    void jobsLoaded(QSharedPointer<IElanceJobsData>) const;

private slots:
    void processAuthorizeReply(QNetworkReply * reply);
    void processTokensReply();
    void processJobsReply();

private:
    void getTokens(const QString & authorizationCode);
    QNetworkReply * post(const QString & url, const QUrlQuery & data);

    static const QString m_authorizeUrl;
    static const QString m_tokenUrl;
    static const QString m_jobsUrl;

    QString m_clientId;
    QString m_clientSecret;
    QString m_redirectUri;

    QString m_accessToken;
    QString m_refreshToken;

    QDialog * m_authorizeDialog;
    QNetworkAccessManager * m_networkManager;
};
}

#endif // ELANCEAPICLIENT_H
