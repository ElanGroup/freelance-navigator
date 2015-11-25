#ifndef UPWORKCATEGORYREADER_H
#define UPWORKCATEGORYREADER_H

#include <QSharedPointer>

class QNetworkReply;

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkCategory;

class UpworkCategoryReader
{
public:
    UpworkCategoryReader();
    UpworkCategoryReader(const UpworkCategoryReader &) = delete;
    UpworkCategoryReader & operator=(const UpworkCategoryReader &) = delete;

    QList<QSharedPointer<UpworkCategory>> readCategories(QNetworkReply * reply) const;

private:
    UpworkCategory * getCategory(const QJsonValue & categoryValue) const;
    bool checkIsValidCategory(const UpworkCategory * const category) const;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKCATEGORYREADER_H
