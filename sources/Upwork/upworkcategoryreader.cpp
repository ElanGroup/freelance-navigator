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
        if (categoriesValue.isArray())
        {
            foreach (const QJsonValue & categoryValue, categoriesValue.toArray())
            {
                UpworkCategory * category = getCategory(categoryValue);
                if (checkIsValidCategory(category))
                {
                    categories.append(QSharedPointer<UpworkCategory>(category));
                }
            }
        }
    }
    return categories;
}

UpworkCategory * UpworkCategoryReader::getCategory(const QJsonValue & categoryValue) const
{
    UpworkCategory * category = new UpworkCategory();
    if (categoryValue.isObject())
    {
        QJsonObject categoryObject = categoryValue.toObject();

        QJsonValue idValue = categoryObject["id"];
        if (idValue.isString())
        {
            category->setCategoryId(idValue.toString());
        }

        QJsonValue titleValue = categoryObject["title"];
        if (titleValue.isString())
        {
            category->setTitle(titleValue.toString());
        }

        QJsonValue topicsValue = categoryObject["topics"];
        if (topicsValue.isArray())
        {
            foreach (const QJsonValue & subcategoryValue, topicsValue.toArray())
            {
                UpworkCategory * subcategory = getCategory(subcategoryValue);
                if (checkIsValidCategory(subcategory))
                {
                    category->addSubcategory(QSharedPointer<UpworkCategory>(subcategory));
                }
            }
        }
    }
    return category;
}

bool UpworkCategoryReader::checkIsValidCategory(const UpworkCategory * const category) const
{
    return !category->categoryId().isEmpty() && !category->title().isEmpty();
}
