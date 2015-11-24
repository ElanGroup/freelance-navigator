#include "upworksearchjobparameters.h"

using namespace FreelanceNavigator::Upwork;

UpworkSearchJobParameters::UpworkSearchJobParameters(const QString & categoryId,
                                                     const QString & searchQuery) :
    m_categoryId(categoryId),
    m_searchQuery(searchQuery)
{
}

UpworkSearchJobParameters::UpworkSearchJobParameters(UpworkSearchJobParameters && other) :
    m_categoryId(std::move(other.m_categoryId)),
    m_searchQuery(std::move(other.m_searchQuery))
{
}

QString UpworkSearchJobParameters::categoryId() const
{
    return m_categoryId;
}

QString UpworkSearchJobParameters::searchQuery() const
{
    return m_searchQuery;
}
