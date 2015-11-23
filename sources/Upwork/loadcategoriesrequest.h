#ifndef LOADCATEGORIESREQUEST_H
#define LOADCATEGORIESREQUEST_H

#include "upworkapirequest.h"

namespace FreelanceNavigator
{
namespace Upwork
{

class LoadCategoriesRequest : public UpworkApiRequest
{
    Q_OBJECT
public:
    explicit LoadCategoriesRequest(const OAuthParameters & parameters,
                                   QNetworkAccessManager * networkManager,
                                   QObject * parent = 0);
    LoadCategoriesRequest(const LoadCategoriesRequest &) = delete;
    LoadCategoriesRequest & operator=(const LoadCategoriesRequest &) = delete;

protected:
    virtual QString path() const override;
    virtual QNetworkAccessManager::Operation operation() const override;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // LOADCATEGORIESREQUEST_H
