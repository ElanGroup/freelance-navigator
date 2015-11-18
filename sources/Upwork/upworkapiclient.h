#ifndef UPWORKAPICLIENT_H
#define UPWORKAPICLIENT_H

#include <QObject>

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
    explicit UpworkApiClient(UpworkSettings * settings, QObject * parent = 0);
    UpworkApiClient(const UpworkApiClient &) = delete;
    UpworkApiClient & operator=(const UpworkApiClient &) = delete;

    void initialize();
    void loadCategories();

signals:
    void initialized() const;

private slots:
    void processGetRequestTokenResult();

private:
    static const QString m_callbackUrl;

    UpworkSettings * m_settings;
    QNetworkAccessManager * m_networkManager;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKAPICLIENT_H
