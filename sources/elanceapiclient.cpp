#include <QDialog>
#include <QLayout>
#include <QWebView>
#include <QNetworkReply>
#include <QSettings>
#include <QUrl>
#include <QUrlQuery>
#include "elanceapiclient.h"
#include "elancedatareader.h"
#include "elancetokensdata.h"

using namespace FreelanceNavigator;

const QString ElanceApiClient::m_authorizeUrl("https://api.elance.com/api2/oauth/authorize");
const QString ElanceApiClient::m_tokenUrl("https://api.elance.com/api2/oauth/token");

ElanceApiClient::ElanceApiClient(QObject * parent) :
    QObject(parent),
    m_authorizeDialog(0),
    m_networkManager(new QNetworkAccessManager(this))
{
}

ElanceApiClient::~ElanceApiClient()
{
}

bool ElanceApiClient::readSettings()
{
    QSettings settings;
    settings.beginGroup("Elance API");

    QVariant key = settings.value("API Key");
    if (!key.isValid())
    {
        return false;
    }
    m_clientId = key.toString();

    QVariant code = settings.value("Secret Code");
    if (!code.isValid())
    {
        return false;
    }
    m_clientSecret = code.toString();

    QVariant uri = settings.value("Redirect URI");
    if (!uri.isValid())
    {
        return false;
    }
    m_redirectUri = uri.toString();

    return true;
}

bool ElanceApiClient::authorize()
{
    m_authorizeDialog = new QDialog();
    m_authorizeDialog->setWindowFlags(m_authorizeDialog->windowFlags() &
                                      ~Qt::WindowContextHelpButtonHint);
    m_authorizeDialog->setWindowState(m_authorizeDialog->windowState() | Qt::WindowMaximized);
    m_authorizeDialog->setWindowTitle(tr("Freelance Navigator - Elance Authorization"));
    QWebView * webView = new QWebView(m_authorizeDialog);
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(webView);
    m_authorizeDialog->setLayout(layout);
    connect(webView->page()->networkAccessManager(), &QNetworkAccessManager::finished,
            this, &ElanceApiClient::processAuthorizeReply);
    QUrl url(m_authorizeUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("client_id", m_clientId);
    urlQuery.addQueryItem("response_type", "code");
    url.setQuery(urlQuery);
    webView->load(url);
    int result = m_authorizeDialog->exec();
    delete m_authorizeDialog;
    m_authorizeDialog = 0;
    return result == QDialog::Accepted;
}

void ElanceApiClient::processAuthorizeReply(QNetworkReply * reply)
{
    QString replyUrl = reply->url().toString();
    if (replyUrl.startsWith(m_redirectUri))
    {
        int index = replyUrl.indexOf("code=");
        if (index == -1)
        {
            if (m_authorizeDialog)
            {
                m_authorizeDialog->reject();
            }
        }
        else
        {
            getTokens(replyUrl.mid(index + 5));
        }
    }
    reply->deleteLater();
}

void ElanceApiClient::getTokens(const QString & authorizationCode)
{
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &ElanceApiClient::processTokensReply);

    QUrlQuery data;
    data.addQueryItem("code", authorizationCode);
    data.addQueryItem("client_id", m_clientId);
    data.addQueryItem("client_secret", m_clientSecret);
    data.addQueryItem("grant_type", "authorization_code");
    post(m_tokenUrl, data);
}

void ElanceApiClient::processTokensReply(QNetworkReply * reply)
{
    disconnect(m_networkManager, &QNetworkAccessManager::finished,
               this, &ElanceApiClient::processTokensReply);

    if (reply->error() == QNetworkReply::NoError)
    {
        QSharedPointer<ElanceTokensData> tokensData =
            ElanceDataReader::readTokensData(reply->readAll());
        reply->deleteLater();
        if (!tokensData->isNull())
        {
            m_accessToken = tokensData->accessToken();
            m_refreshToken = tokensData->refreshToken();
            if (m_authorizeDialog)
            {
                m_authorizeDialog->accept();
                return;
            }
        }
    }

    if (m_authorizeDialog)
    {
        m_authorizeDialog->reject();
    }
}

void ElanceApiClient::post(const QString & url, const QUrlQuery & data)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    m_networkManager->post(request, data.toString(QUrl::FullyEncoded).toUtf8());
}
