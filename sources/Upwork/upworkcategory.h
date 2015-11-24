#ifndef UPWORKCATEGORY_H
#define UPWORKCATEGORY_H

#include <QString>
#include <QSharedPointer>

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkCategory
{
public:
    UpworkCategory();
    UpworkCategory(const UpworkCategory &) = delete;
    UpworkCategory & operator=(const UpworkCategory &) = delete;

    QString categoryId() const;
    void setCategoryId(const QString & categoryId);
    QString title() const;
    void setTitle(const QString & title);
    const QList<QSharedPointer<UpworkCategory>> & subcategories() const;
    void addSubcategory(const QSharedPointer<UpworkCategory> & subcategory);

private:
    QString m_categoryId;
    QString m_title;
    QList<QSharedPointer<UpworkCategory>> m_subcategories;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKCATEGORY_H
