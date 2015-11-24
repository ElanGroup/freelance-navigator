#include "requestfactory.h"
#include "upworksettings.h"
#include "oauthparameters.h"
#include "getrequesttokenrequest.h"
#include "getaccesstokenrequest.h"
#include "loadcategoriesrequest.h"
#include "searchjobsrequest.h"

using namespace FreelanceNavigator::Upwork;

RequestFactory::RequestFactory(UpworkSettings * settings, QNetworkAccessManager * networkManager) :
    m_settings(settings),
    m_networkManager(networkManager)
{
}

GetRequestTokenRequest * RequestFactory::createGetRequestTokenRequest() const
{
    OAuthParameters parameters(m_settings->upworkKey(), m_settings->upworkSecret());
    return new GetRequestTokenRequest(parameters, m_networkManager, m_networkManager);
}

GetAccessTokenRequest * RequestFactory::createGetAccessTokenRequest(const QString & requestToken,
                                                                    const QString & requestTokenSecret,
                                                                    const QString & verificationCode) const
{
    OAuthParameters parameters(m_settings->upworkKey(),
                               m_settings->upworkSecret(),
                               requestToken,
                               requestTokenSecret,
                               verificationCode);
    return new GetAccessTokenRequest(parameters, m_networkManager, m_networkManager);
}

LoadCategoriesRequest * RequestFactory::createLoadCategoriesRequest(const QString & accessToken,
                                                                    const QString & accessTokenSecret) const
{
    OAuthParameters parameters(m_settings->upworkKey(),
                               m_settings->upworkSecret(),
                               accessToken,
                               accessTokenSecret);
    return new LoadCategoriesRequest(parameters, m_networkManager, m_networkManager);
}

SearchJobsRequest * RequestFactory::createSearchJobsRequest(const UpworkSearchJobParameters & parameters,
                                                            const QString & accessToken,
                                                            const QString & accessTokenSecret) const
{
    OAuthParameters authenticationParameters(m_settings->upworkKey(),
                                             m_settings->upworkSecret(),
                                             accessToken,
                                             accessTokenSecret);
    return new SearchJobsRequest(parameters,
                                 authenticationParameters,
                                 m_networkManager,
                                 m_networkManager);
}
