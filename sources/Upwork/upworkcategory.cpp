#include "upworkcategory.h"

using namespace FreelanceNavigator::Upwork;

UpworkCategory::UpworkCategory()
{
}

QString UpworkCategory::categoryId() const
{
    return m_categoryId;
}

void UpworkCategory::setCategoryId(const QString & categoryId)
{
    m_categoryId = categoryId;
}

QString UpworkCategory::title() const
{
    return m_title;
}

void UpworkCategory::setTitle(const QString & title)
{
    m_title = title;
}

const QList<QSharedPointer<UpworkCategory>> & UpworkCategory::subcategories() const
{
    return m_subcategories;
}

void UpworkCategory::addSubcategory(const QSharedPointer<UpworkCategory> & subcategory)
{
    m_subcategories.append(subcategory);
}
