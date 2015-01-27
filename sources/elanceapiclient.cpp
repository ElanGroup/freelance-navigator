#include <QDialog>
#include <QLayout>
#include <QWebView>
#include <QNetworkReply>
#include <QSettings>
#include <QUrl>
#include <QUrlQuery>
#include "elanceapiclient.h"
#include "elancedatareader.h"
#include "ielancetokens.h"
#include "ielancejobspage.h"
#include "ielanceerror.h"
#include "cookiejar.h"

using namespace FreelanceNavigator;

const QString ElanceApiClient::m_authorizeUrl("https://api.elance.com/api2/oauth/authorize");
const QString ElanceApiClient::m_tokenUrl("https://api.elance.com/api2/oauth/token");
const QString ElanceApiClient::m_categoriesUrl("https://api.elance.com/api2/categories");
const QString ElanceApiClient::m_jobsUrl("https://api.elance.com/api2/jobs");

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
    webView->page()->networkAccessManager()->setCookieJar(new CookieJar());
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
    QUrlQuery data;
    data.addQueryItem("code", authorizationCode);
    data.addQueryItem("client_id", m_clientId);
    data.addQueryItem("client_secret", m_clientSecret);
    data.addQueryItem("grant_type", "authorization_code");
    QNetworkReply * reply = post(m_tokenUrl, data);
    connect(reply, &QNetworkReply::finished, this, &ElanceApiClient::processTokensReply);
}

void ElanceApiClient::processTokensReply()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT(reply);
    QNetworkReply::NetworkError error = reply->error();
    QSharedPointer<IElanceTokens> tokens = ElanceDataReader::readTokens(reply->readAll());
    reply->deleteLater();
    if (error == QNetworkReply::NoError && tokens->isValid())
    {
        m_accessToken = tokens->accessToken();
        m_refreshToken = tokens->refreshToken();
        if (m_authorizeDialog)
        {
            m_authorizeDialog->accept();
            return;
        }
    }

    if (m_authorizeDialog)
    {
        m_authorizeDialog->reject();
    }
}

void ElanceApiClient::loadCategories()
{
    QUrl url(m_categoriesUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("access_token", m_accessToken);
    url.setQuery(urlQuery);
    QNetworkRequest request(url);
    QNetworkReply * reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ElanceApiClient::processCategoriesReply);
}

void ElanceApiClient::processCategoriesReply()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT(reply);
    if (reply->error() == QNetworkReply::NoError)
    {
        QList<QSharedPointer<IElanceCategory> > categories =
            ElanceDataReader::readCategories(reply->readAll());
        emit categoriesLoaded(categories);
    }
    else
    {
        processError(reply);
    }
    reply->deleteLater();
}

void ElanceApiClient::loadJobs(int category, const QList<int> & subcategories)
{
    QUrl url(m_jobsUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("access_token", m_accessToken);
    urlQuery.addQueryItem("rpp", QString::number(m_jobsNumberPerPage));
    urlQuery.addQueryItem("catFilter", QString::number(category));
    if (!subcategories.isEmpty())
    {
        QStringList subcategoriesList;
        for (int i = 0; i < subcategories.count(); ++i)
        {
            subcategoriesList << QString::number(subcategories[i]);
        }
        urlQuery.addQueryItem("subcatFilter", subcategoriesList.join(','));
    }
    url.setQuery(urlQuery);
    QNetworkRequest request(url);
    QNetworkReply * reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ElanceApiClient::processJobsReply);
}

void ElanceApiClient::processJobsReply()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender());
    Q_ASSERT(reply);
    if (reply->error() == QNetworkReply::NoError)
    {
        QSharedPointer<IElanceJobsPage> jobsPage =
            ElanceDataReader::readJobsPage(reply->readAll());
        if (jobsPage->isValid())
        {
            emit jobsLoaded(jobsPage);
        }
    }
    else
    {
        processError(reply);
    }
    reply->deleteLater();
}

QNetworkReply * ElanceApiClient::post(const QString & url, const QUrlQuery & data)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    return m_networkManager->post(request, data.toString(QUrl::FullyEncoded).toUtf8());
}

void ElanceApiClient::processError(QNetworkReply * reply)
{
    QList<QSharedPointer<IElanceError> > errors = ElanceDataReader::readErrors(reply->readAll());
    if (reply->error() == QNetworkReply::TimeoutError)
    {
        emit error(tr("Connection error. Please check your internet connection."));
        return;
    }
    emit error(tr("Unknown error"));
}
