#ifndef ELANCEAPICLIENT_H
#define ELANCEAPICLIENT_H

#include <QObject>

class QNetworkReply;
class QDialog;
class QNetworkAccessManager;
class QUrlQuery;

namespace FreelanceNavigator
{
class ElanceApiClient : public QObject
{
    Q_OBJECT
public:
    explicit ElanceApiClient(QObject * parent = 0);
    ~ElanceApiClient();

    bool readSettings();
    bool authorize();
    void getJobs();

signals:

private slots:
    void processAuthorizeReply(QNetworkReply * reply);
    void processTokensReply(QNetworkReply * reply);
    void processJobsReply(QNetworkReply * reply);

private:
    void getTokens(const QString & authorizationCode);
    void post(const QString & url, const QUrlQuery & data);

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
