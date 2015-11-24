#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "upworkcategoryreader.h"
#include "upworkcategory.h"

using namespace FreelanceNavigator::Upwork;

UpworkCategoryReader::UpworkCategoryReader()
{
}

QList<QSharedPointer<UpworkCategory>> UpworkCategoryReader::readCategories(QNetworkReply * reply) const
{
    QList<QSharedPointer<UpworkCategory>> categories;
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if (!document.isNull() && document.isObject())
    {
        QJsonValue categoriesValue = document.object()["categories"];
        if (!categoriesValue.isUndefined() && categoriesValue.isArray())
        {
            foreach (const QJsonValue & categoryValue, categoriesValue.toArray())
            {
                QSharedPointer<UpworkCategory> category = getCategory(categoryValue);
                if (checkIsValidCategory(*category))
                {
                    categories.append(category);
                }
            }
        }
    }
    return categories;
}

QSharedPointer<UpworkCategory> UpworkCategoryReader::getCategory(const QJsonValue & categoryValue) const
{
    UpworkCategory * category = new UpworkCategory();
    if (categoryValue.isObject())
    {
        QJsonObject categoryObject = categoryValue.toObject();

        QJsonValue idValue = categoryObject["id"];
        if (!idValue.isUndefined() && idValue.isString())
        {
            category->setCategoryId(idValue.toString());
        }

        QJsonValue titleValue = categoryObject["title"];
        if (!titleValue.isUndefined() && titleValue.isString())
        {
            category->setTitle(titleValue.toString());
        }

        QJsonValue topicsValue = categoryObject["topics"];
        if (!topicsValue.isUndefined() && topicsValue.isArray())
        {
            foreach (const QJsonValue & subcategoryValue, topicsValue.toArray())
            {
                QSharedPointer<UpworkCategory> subcategory = getCategory(subcategoryValue);
                if (checkIsValidCategory(*subcategory))
                {
                    category->addSubcategory(subcategory);
                }
            }
        }
    }
    return QSharedPointer<UpworkCategory>(category);
}

bool UpworkCategoryReader::checkIsValidCategory(const UpworkCategory & category) const
{
    return !category.categoryId().isEmpty() && !category.title().isEmpty();
}
