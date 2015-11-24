#ifndef UPWORKSEARCHJOBPARAMETERS_H
#define UPWORKSEARCHJOBPARAMETERS_H

#include <QString>

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkSearchJobParameters
{
public:
    UpworkSearchJobParameters(const QString & categoryId, const QString & searchQuery);
    UpworkSearchJobParameters(const UpworkSearchJobParameters &) = delete;
    UpworkSearchJobParameters & operator=(const UpworkSearchJobParameters &) = delete;
    UpworkSearchJobParameters(UpworkSearchJobParameters && other);

    QString categoryId() const;
    QString searchQuery() const;

private:
    QString m_categoryId;
    QString m_searchQuery;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKSEARCHJOBPARAMETERS_H
