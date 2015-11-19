#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "upworkapiclient.h"
#include "upworksettings.h"
#include "upworkerrorhandler.h"
#include "getrequesttokenrequest.h"
#include "upworktokenreader.h"

using namespace FreelanceNavigator::Upwork;

const QString UpworkApiClient::m_callbackUrl("http://127.0.0.1:3000/");

UpworkApiClient::UpworkApiClient(UpworkSettings * settings, QObject * parent) :
    QObject(parent),
    m_settings(settings),
    m_networkManager(new QNetworkAccessManager(this))
{
}

void UpworkApiClient::initialize()
{
    GetRequestTokenRequest * request = new GetRequestTokenRequest(m_settings->upworkKey(),
                                                                  m_settings->upworkSecret(),
                                                                  m_callbackUrl,
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
    }
    request->deleteLater();
}

void UpworkApiClient::loadCategories()
{

}
