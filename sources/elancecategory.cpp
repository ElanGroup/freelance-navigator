#include "elancecategory.h"

using namespace FreelanceNavigator;

ElanceCategory::ElanceCategory() : m_categoryId(-1)
{
}

ElanceCategory::~ElanceCategory()
{
}

bool ElanceCategory::isValid() const
{
    return m_categoryId != -1 && !m_name.isEmpty();
}

int ElanceCategory::categoryId() const
{
    return m_categoryId;
}

void ElanceCategory::setCategoryId(int categoryId)
{
    m_categoryId = categoryId;
}

QString ElanceCategory::name() const
{
    return m_name;
}

void ElanceCategory::setName(const QString & name)
{
    m_name = name;
}

const QList<QSharedPointer<IElanceCategory> > & ElanceCategory::subcategories() const
{
    return m_subcategories;
}

void ElanceCategory::addSubcategory(const QSharedPointer<IElanceCategory> & subcategory)
{
    m_subcategories.append(subcategory);
}
