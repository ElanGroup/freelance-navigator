#ifndef ELANCECATEGORY_H
#define ELANCECATEGORY_H

#include "ielancecategory.h"

namespace FreelanceNavigator
{
class ElanceCategory : public IElanceCategory
{
public:
    ElanceCategory();
    ~ElanceCategory();

    virtual bool isValid() const;
    virtual int categoryId() const;
    void setCategoryId(int categoryId);
    virtual QString name() const;
    void setName(const QString & name);
    virtual const QList<QSharedPointer<IElanceCategory> > & subcategories() const;
    void addSubcategory(const QSharedPointer<IElanceCategory> & subcategory);

private:
    ElanceCategory(const ElanceCategory &);
    ElanceCategory & operator=(const ElanceCategory &);

    int m_categoryId;
    QString m_name;
    QList<QSharedPointer<IElanceCategory> > m_subcategories;
};
}

#endif // ELANCECATEGORY_H
