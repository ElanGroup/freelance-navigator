#include <QStringList>
#include "upworksearchjobparameters.h"

using namespace FreelanceNavigator::Upwork;

UpworkSearchJobParameters::UpworkSearchJobParameters(const QString & categoryId,
                                                     const QString & searchQuery) :
    m_category(categoryId),
    m_searchQuery(searchQuery)
{
}

UpworkSearchJobParameters::UpworkSearchJobParameters(UpworkSearchJobParameters && other) :
    m_category(std::move(other.m_category)),
    m_subcategories(std::move(other.m_subcategories)),
    m_searchQuery(std::move(other.m_searchQuery))
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
