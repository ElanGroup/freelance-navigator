#ifndef IELANCECATEGORY_H
#define IELANCECATEGORY_H

#include <QList>
#include <QSharedPointer>
#include "ielancedata.h"

namespace FreelanceNavigator
{
class IElanceCategory : public IElanceData
{
public:
    virtual ~IElanceCategory() {}
    virtual int categoryId() const = 0;
    virtual QString name() const = 0;
    virtual const QList<QSharedPointer<IElanceCategory> > & subcategories() const = 0;
};
}

#endif // IELANCECATEGORY_H
