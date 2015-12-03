#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include "upworkapiclient.h"
#include "upworksettings.h"
#include "upworkerrorhandler.h"
#include "getrequesttokenrequest.h"
#include "upworktokenreader.h"
#include "authorizationdialog.h"
#include "requestfactory.h"
#include "getaccesstokenrequest.h"
#include "loadcategoriesrequest.h"
#include "upworkcategoryreader.h"
#include "searchjobsrequest.h"
#include "upworkjobreader.h"
#include "upworkjobpage.h"

using namespace FreelanceNavigator::Upwork;

const QString UpworkApiClient::m_authorizationUrl("https://www.upwork.com/services/api/auth");

UpworkApiClient::UpworkApiClient(UpworkSettings * settings, QWidget * parent) :
    QObject(parent),
    m_settings(settings),
    m_requestFactory(new RequestFactory(settings, new QNetworkAccessManager(this)))
{
}

UpworkApiClient::~UpworkApiClient()
{
    delete m_requestFactory;
}

bool UpworkApiClient::isLoggedIn() const
{
    return !m_accessToken.isEmpty() && !m_accessTokenSecret.isEmpty();
}

void UpworkApiClient::initialize()
{
    m_accessToken = m_settings->upworkAccessToken();
    m_accessTokenSecret = m_settings->upworkAccessTokenSecret();
    if (!m_accessToken.isEmpty() && !m_accessTokenSecret.isEmpty())
    {
        emit initialized();
        return;
    }

    auto request = m_requestFactory->createGetRequestTokenRequest();
    connect(request, &ApiRequest::finished, this, &UpworkApiClient::processGetRequestTokenResult);
    request->submit();
}

void UpworkApiClient::processGetRequestTokenResult()
{
    GetRequestTokenRequest * request = qobject_cast<GetRequestTokenRequest *>(sender());
    Q_ASSERT(request);
    UpworkErrorHandler errorHandler(request->reply());
    if (errorHandler.hasError())
    {
        processError(errorHandler);
    }
    else
    {
        UpworkTokenReader tokenReader(request->reply());
        m_requestToken = tokenReader.readToken();
        m_requestTokenSecret = tokenReader.readTokenSecret();
        authorize();
    }
    request->deleteLater();
}

void UpworkApiClient::authorize() const
{
    QUrl url(m_authorizationUrl);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("oauth_token"), m_requestToken);
    url.setQuery(urlQuery);
    AuthorizationDialog authorizationDialog(QStringLiteral("Upwork"),
                                            url,
                                            qobject_cast<QWidget *>(parent()));
    connect(&authorizationDialog, &AuthorizationDialog::urlChanged,
            this, &UpworkApiClient::processAuthorizationRedirect);
    int result = authorizationDialog.exec();
    if (result == QDialog::Accepted)
    {
        getAccessToken();
    }
    else
    {
        emit warning(UpworkApiWarning::AuthorizationRequired);
    }
}

void UpworkApiClient::processAuthorizationRedirect(const QUrl & url)
{
    QString query = url.query();
    if (!query.contains(QStringLiteral("oauth_verifier")))
    {
        return;
    }

    QUrlQuery urlQuery(query);
    m_verificationCode = urlQuery.queryItemValue(QStringLiteral("oauth_verifier"));

    AuthorizationDialog * authorizationDialog = qobject_cast<AuthorizationDialog *>(sender());
    Q_ASSERT(authorizationDialog);
    authorizationDialog->accept();
}

void UpworkApiClient::getAccessToken() const
{
    auto request = m_requestFactory->createGetAccessTokenRequest(m_requestToken,
                                                                 m_requestTokenSecret,
                                                                 m_verificationCode);
    connect(request, &ApiRequest::finished, this, &UpworkApiClient::processGetAccessTokenResult);
    request->submit();
}

void UpworkApiClient::processGetAccessTokenResult()
{
    GetAccessTokenRequest * request = qobject_cast<GetAccessTokenRequest *>(sender());
    Q_ASSERT(request);
    UpworkErrorHandler errorHandler(request->reply());
    if (errorHandler.hasError())
    {
        processError(errorHandler);
    }
    else
    {
        UpworkTokenReader tokenReader(request->reply());
        m_accessToken = tokenReader.readToken();
        m_accessTokenSecret = tokenReader.readTokenSecret();
        m_settings->saveUpworkAccessToken(m_accessToken, m_accessTokenSecret);
        emit initialized();
    }
    request->deleteLater();
}

void UpworkApiClient::loadCategories()
{
    auto request = m_requestFactory->createLoadCategoriesRequest(m_accessToken,
                                                                 m_accessTokenSecret);
    connect(request, &ApiRequest::finished, this, &UpworkApiClient::processLoadCategoriesResult);
    request->submit();
}

void UpworkApiClient::processLoadCategoriesResult()
{
    LoadCategoriesRequest * request = qobject_cast<LoadCategoriesRequest *>(sender());
    Q_ASSERT(request);
    UpworkErrorHandler errorHandler(request->reply());
    if (errorHandler.hasError())
    {
        processError(errorHandler);
    }
    else
    {
        UpworkCategoryReader categoryReader;
        emit categoriesLoaded(categoryReader.readCategories(request->reply()));
    }
    request->deleteLater();
}

void UpworkApiClient::searchJobs(const UpworkSearchJobParameters & parameters)
{
    m_stopJobSearching = false;
    auto request = m_requestFactory->createSearchJobsRequest(parameters,
                                                             m_accessToken,
                                                             m_accessTokenSecret);
    connect(request, &ApiRequest::finished, this, &UpworkApiClient::processSearchJobsResult);
    request->submit();
}

void UpworkApiClient::processSearchJobsResult()
{
    SearchJobsRequest * request = qobject_cast<SearchJobsRequest *>(sender());
    Q_ASSERT(request);
    UpworkErrorHandler errorHandler(request->reply());
    if (errorHandler.hasError())
    {
        processError(errorHandler);
    }
    else if (!m_stopJobSearching)
    {
        UpworkJobReader jobReader;
        std::unique_ptr<UpworkJobPage> jobPage = jobReader.readJobPage(request->reply());
        emit jobsLoaded(jobPage->jobs());
        if (jobPage->isValid())
        {
            if (jobPage->offset() + jobPage->count() >= jobPage->total())
            {
                emit jobSearchingFinished();
            }
            else
            {
                request->setOffset(jobPage->offset() + jobPage->count());
                request->submit();
                // Return to prevent request deleting.
                return;
            }
        }
        else
        {
            Q_ASSERT(true);
            emit jobSearchingFinished();
        }
    }
    request->deleteLater();
}

void UpworkApiClient::stopSearchJobs()
{
    m_stopJobSearching = true;
}

void UpworkApiClient::logOut()
{
    if (!isLoggedIn())
    {
        return;
    }
    stopSearchJobs();
    m_accessToken.clear();
    m_accessTokenSecret.clear();
    m_settings->removeUpworkAccessToken();
}

void UpworkApiClient::processError(const UpworkErrorHandler & errorHandler)
{
    if (errorHandler.isConnectionError())
    {
        emit error(UpworkApiError::ConnectionError);
    }
    else if (errorHandler.isAuthenticationError())
    {
        logOut();
        emit error(UpworkApiError::AuthenticationError);
    }
    else
    {
        emit error(UpworkApiError::ServiceError);
    }
}
