#ifndef CATEGORIESREQUEST_H
#define CATEGORIESREQUEST_H

#include "datarequest.h"

namespace FreelanceNavigator
{
class IElanceCategory;

class CategoriesRequest : public DataRequest
{
    Q_OBJECT
public:
    explicit CategoriesRequest(const QString & accessToken,
                               QNetworkAccessManager * networkManager,
                               QObject * parent);
    virtual ~CategoriesRequest();

    QList<QSharedPointer<IElanceCategory> > categories() const;

protected:
    virtual void doSubmit() const;
    virtual void readData(const QByteArray & data);

private:
    Q_DISABLE_COPY(CategoriesRequest)

    QList<QSharedPointer<IElanceCategory> > m_categories;

    static const QString m_categoriesUrl;
};
}

#endif // CATEGORIESREQUEST_H
