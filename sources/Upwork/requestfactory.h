#ifndef REQUESTFACTORY_H
#define REQUESTFACTORY_H

class QNetworkAccessManager;
class QString;

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkSettings;
class GetRequestTokenRequest;
class GetAccessTokenRequest;
class LoadCategoriesRequest;
class SearchJobsRequest;
class UpworkSearchJobParameters;

class RequestFactory
{
public:
    RequestFactory(UpworkSettings * settings, QNetworkAccessManager * networkManager);
    RequestFactory(const RequestFactory &) = delete;
    RequestFactory & operator=(const RequestFactory &) = delete;

    GetRequestTokenRequest * createGetRequestTokenRequest() const;
    GetAccessTokenRequest * createGetAccessTokenRequest(const QString & requestToken,
                                                        const QString & requestTokenSecret,
                                                        const QString & verificationCode) const;
    LoadCategoriesRequest * createLoadCategoriesRequest(const QString & accessToken,
                                                        const QString & accessTokenSecret) const;
    SearchJobsRequest * createSearchJobsRequest(const UpworkSearchJobParameters & parameters,
                                                const QString & accessToken,
                                                const QString & accessTokenSecret) const;

private:
    UpworkSettings * m_settings;
    QNetworkAccessManager * m_networkManager;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // REQUESTFACTORY_H
