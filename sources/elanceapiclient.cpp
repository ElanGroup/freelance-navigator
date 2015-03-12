#include <QDialog>
#include <QLayout>
#include <QWebView>
#include <QSettings>
#include <QNetworkDiskCache>
#include <QStandardPaths>
#include "elanceapiclient.h"
#include "cookiejar.h"
#include "ielanceerror.h"
#include "ielancejobspage.h"
#include "gettokensrequest.h"
#include "refreshtokensrequest.h"
#include "categoriesrequest.h"
#include "jobsrequest.h"

using namespace FreelanceNavigator;

const QString ElanceApiClient::m_authorizeUrl("https://api.elance.com/api2/oauth/authorize");

ElanceApiClient::ElanceApiClient(QObject * parent) :
    QObject(parent),
    m_isTokensRefreshingActive(false),
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

    QVariant accessToken = settings.value("Access Token");
    if (accessToken.isValid())
    {
        m_accessToken = accessToken.toString();
    }

    QVariant refreshToken = settings.value("Refresh Token");
    if (refreshToken.isValid())
    {
        m_refreshToken = refreshToken.toString();
    }

    return true;
}

bool ElanceApiClient::authorize()
{
    if (!m_accessToken.isEmpty() && !m_refreshToken.isEmpty())
    {
        return true;
    }

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
    QNetworkDiskCache * diskCache = new QNetworkDiskCache();
    diskCache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    webView->page()->networkAccessManager()->setCache(diskCache);
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

void ElanceApiClient::logout()
{
    m_accessToken.clear();
    m_refreshToken.clear();

    QSettings settings;
    settings.beginGroup("Elance API");
    settings.remove("Access Token");
    settings.remove("Refresh Token");

    CookieJar::clear();
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
    GetTokensRequest * request = new GetTokensRequest(authorizationCode,
                                                      m_clientId,
                                                      m_clientSecret,
                                                      m_networkManager,
                                                      this);
    connect(request, &ElanceApiRequest::finished, this, &ElanceApiClient::processGetTokensResult);
    request->submit();
}

void ElanceApiClient::processGetTokensResult(bool isOk)
{
    GetTokensRequest * request = qobject_cast<GetTokensRequest *>(sender());
    Q_ASSERT(request);
    if (isOk)
    {
        m_accessToken = request->accessToken();
        m_refreshToken = request->refreshToken();
        saveTokens();
        if (m_authorizeDialog)
        {
            m_authorizeDialog->accept();
        }
    }
    else if (m_authorizeDialog)
    {
        m_authorizeDialog->reject();
    }
    request->deleteLater();
}

void ElanceApiClient::refreshTokens()
{
    m_isTokensRefreshingActive = true;
    RefreshTokensRequest * request = new RefreshTokensRequest(m_clientId,
                                                              m_clientSecret,
                                                              m_refreshToken,
                                                              m_networkManager,
                                                              this);
    connect(request, &ElanceApiRequest::finished,
            this, &ElanceApiClient::processRefreshTokensResult);
    request->submit();
}

void ElanceApiClient::processRefreshTokensResult(bool isOk)
{
    RefreshTokensRequest * request = qobject_cast<RefreshTokensRequest *>(sender());
    Q_ASSERT(request);
    if (isOk)
    {
        m_accessToken = request->accessToken();
        m_refreshToken = request->refreshToken();
        saveTokens();
        m_isTokensRefreshingActive = false;
        processPendingRequests();
    }
    else
    {
        m_isTokensRefreshingActive = false;
        emit this->error(ServiceError);
        m_pendingRequests.clear();
    }
    request->deleteLater();
}

void ElanceApiClient::loadCategories()
{
    CategoriesRequest * request = new CategoriesRequest(m_accessToken, m_networkManager, this);
    connect(request, &ElanceApiRequest::finished, this, &ElanceApiClient::processCategoriesResult);
    if (m_isTokensRefreshingActive)
    {
        m_pendingRequests.append(request);
    }
    else
    {
        request->submit();
    }
}

void ElanceApiClient::processCategoriesResult(bool isOk)
{
    CategoriesRequest * request = qobject_cast<CategoriesRequest *>(sender());
    Q_ASSERT(request);
    if (isOk)
    {
        emit categoriesLoaded(request->categories());
        request->deleteLater();
    }
    else
    {
        processError(request);
    }
}

void ElanceApiClient::loadJobs(int category,
                               const QList<int> & subcategories,
                               const QString & keywords,
                               int page)
{
    JobsRequest * request = new JobsRequest(category,
                                            subcategories,
                                            keywords,
                                            page,
                                            m_accessToken,
                                            m_networkManager,
                                            this);
    connect(request, &ElanceApiRequest::finished, this, &ElanceApiClient::processJobsResult);
    if (m_isTokensRefreshingActive)
    {
        m_pendingRequests.append(request);
    }
    else
    {
        request->submit();
    }
}

void ElanceApiClient::processJobsResult(bool isOk)
{
    JobsRequest * request = qobject_cast<JobsRequest *>(sender());
    Q_ASSERT(request);
    if (isOk)
    {
        QSharedPointer<IElanceJobsPage> jobsPage = request->jobsPage();
        if (jobsPage->isValid())
        {
            emit jobsLoaded(jobsPage);
        }
        request->deleteLater();
    }
    else
    {
        processError(request);
    }
}

void ElanceApiClient::saveTokens() const
{
    QSettings settings;
    settings.beginGroup("Elance API");
    settings.setValue("Access Token", m_accessToken);
    settings.setValue("Refresh Token", m_refreshToken);
}

void ElanceApiClient::processPendingRequests()
{
    while (!m_pendingRequests.isEmpty())
    {
        DataRequest * request = m_pendingRequests.takeFirst();
        request->setAccessToken(m_accessToken);
        request->submit();
    }
}

void ElanceApiClient::processError(DataRequest * request)
{
    if (request->error() == QNetworkReply::TimeoutError)
    {
        emit error(ConnectionError);
        request->deleteLater();
        return;
    }
    if (request->error() == QNetworkReply::AuthenticationRequiredError &&
        checkErrorExists(request->errors(), "invalid_token"))
    {
        m_pendingRequests.append(request);
        if (!m_isTokensRefreshingActive)
        {
            refreshTokens();
        }
        return;
    }
    emit error(UnknownError);
    request->deleteLater();
}

bool ElanceApiClient::checkErrorExists(const QList<QSharedPointer<IElanceError> > & errors,
                                       const QString & errorCode)
{
    for (int i = 0; i < errors.count(); ++i)
    {
        if (errors.at(i)->code().contains(errorCode))
        {
            return true;
        }
    }
    return false;
}
