#ifndef UPWORKSEARCHJOBPARAMETERS_H
#define UPWORKSEARCHJOBPARAMETERS_H

#include <QString>
#include "upworkenums.h"

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkSearchJobParameters
{
public:
    UpworkSearchJobParameters(const QString & category,
                              const QString & searchQuery,
                              PostedDateRange postedDateRange);
    UpworkSearchJobParameters(const UpworkSearchJobParameters &) = delete;
    UpworkSearchJobParameters & operator=(const UpworkSearchJobParameters &) = delete;
    UpworkSearchJobParameters(UpworkSearchJobParameters && other);
    UpworkSearchJobParameters & operator=(UpworkSearchJobParameters && other) = delete;

    QString category() const;
    QStringList subcategories() const;
    void addSubcategory(const QString & subcategory);
    QString searchQuery() const;
    PostedDateRange postedDateRange() const;

private:
    QString m_category;
    QStringList m_subcategories;
    QString m_searchQuery;
    PostedDateRange m_postedDateRange;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKSEARCHJOBPARAMETERS_H
