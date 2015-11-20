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

using namespace FreelanceNavigator::Upwork;

const QString UpworkApiClient::m_authorizationUrl("https://www.upwork.com/services/api/auth");

UpworkApiClient::UpworkApiClient(UpworkSettings * settings, QWidget * parent) :
    QObject(parent),
    m_settings(settings),
    m_networkManager(new QNetworkAccessManager(this))
{
}

void UpworkApiClient::initialize()
{
    GetRequestTokenRequest * request = new GetRequestTokenRequest(m_settings->upworkKey(),
                                                                  m_settings->upworkSecret(),
                                                                  m_networkManager,
                                                                  this);
    connect(request, &ApiRequest::finished, this, &UpworkApiClient::processGetRequestTokenResult);
    request->submit();
}

void UpworkApiClient::processGetRequestTokenResult()
{
    GetRequestTokenRequest * request = qobject_cast<GetRequestTokenRequest *>(sender());
    Q_ASSERT(request);
    UpworkErrorHandler errorHandler(request->reply());
    if (errorHandler.hasErrors())
    {
        emit error(UpworkApiError::ServiceError);
    }
    else
    {
        UpworkTokenReader tokenReader(request->reply());
        m_requestToken = tokenReader.readRequestToken();
        m_requestTokenSecret = tokenReader.readRequestTokenSecret();
        authorize();
    }
    request->deleteLater();
}

void UpworkApiClient::loadCategories()
{

}

void UpworkApiClient::authorize()
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

    }
    else
    {

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
    m_verifier = urlQuery.queryItemValue(QStringLiteral("oauth_verifier"));

    AuthorizationDialog * authorizationDialog = qobject_cast<AuthorizationDialog *>(sender());
    Q_ASSERT(authorizationDialog);
    authorizationDialog->accept();
}
