#include <QStringList>
#include "upworksearchjobparameters.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Upwork;

UpworkSearchJobParameters::UpworkSearchJobParameters(const QString & categoryId,
                                                     const QString & searchQuery,
                                                     PostedDateRange postedDateRange,
                                                     JobType jobType) :
    m_category(categoryId),
    m_searchQuery(searchQuery),
    m_postedDateRange(postedDateRange),
    m_jobType(jobType)
{
}

UpworkSearchJobParameters::UpworkSearchJobParameters(UpworkSearchJobParameters && other) :
    m_category(std::move(other.m_category)),
    m_subcategories(std::move(other.m_subcategories)),
    m_searchQuery(std::move(other.m_searchQuery)),
    m_postedDateRange(other.m_postedDateRange),
    m_jobType(other.m_jobType)
{
}

QString UpworkSearchJobParameters::category() const
{
    return m_category;
}

QStringList UpworkSearchJobParameters::subcategories() const
{
    return m_subcategories;
}

void UpworkSearchJobParameters::addSubcategory(const QString & subcategory)
{
    m_subcategories.append(subcategory);
}

QString UpworkSearchJobParameters::searchQuery() const
{
    return m_searchQuery;
}

PostedDateRange UpworkSearchJobParameters::postedDateRange() const
{
    return m_postedDateRange;
}

JobType UpworkSearchJobParameters::jobType() const
{
    return m_jobType;
}
