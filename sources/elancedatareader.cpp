#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "elancedatareader.h"
#include "elancetokens.h"
#include "elancecategory.h"
#include "elancejobspage.h"
#include "elancejob.h"
#include "elanceerror.h"

using namespace FreelanceNavigator;

ElanceDataReader::ElanceDataReader()
{
}

ElanceDataReader::~ElanceDataReader()
{
}

QSharedPointer<IElanceTokens> ElanceDataReader::readTokens(const QByteArray & data)
{
    ElanceTokens * tokens = new ElanceTokens();

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject dataObject = getDataObject(document);
    if (!dataObject.isEmpty())
    {
        QJsonValue accessToken = dataObject["access_token"];
        if (!accessToken.isUndefined() && accessToken.isString())
        {
            tokens->setAccessToken(accessToken.toString());
        }

        QJsonValue refreshToken = dataObject["refresh_token"];
        if (!refreshToken.isUndefined() && refreshToken.isString())
        {
            tokens->setRefreshToken(refreshToken.toString());
        }
    }

    return QSharedPointer<IElanceTokens>(tokens);
}

QList<QSharedPointer<IElanceCategory> > ElanceDataReader::readCategories(const QByteArray & data)
{
    QList<QSharedPointer<IElanceCategory> > categories;

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject dataObject = getDataObject(document);
    if (!dataObject.isEmpty())
    {
        foreach (const QJsonValue & categoryValue, dataObject)
        {
            QSharedPointer<IElanceCategory> category = getCategory(categoryValue);
            if (category->isValid())
            {
                categories.append(category);
            }
        }
    }

    return categories;
}

QSharedPointer<IElanceJobsPage> ElanceDataReader::readJobsPage(const QByteArray & data)
{
    ElanceJobsPage * jobsPage = new ElanceJobsPage();

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject dataObject = getDataObject(document);
    if (!dataObject.isEmpty())
    {
        QJsonValue totalResults = dataObject["totalResults"];
        if (!totalResults.isUndefined() && totalResults.isDouble())
        {
            jobsPage->setJobsTotal(totalResults.toInt());
        }

        QJsonValue pageResults = dataObject["pageResults"];
        if (!pageResults.isUndefined() && pageResults.isArray())
        {
            QJsonArray jobsArray = pageResults.toArray();
            foreach (const QJsonValue & jobValue, jobsArray)
            {
                QSharedPointer<IElanceJob> job = getJob(jobValue);
                if (job->isValid())
                {
                    jobsPage->addJob(job);
                }
            }
        }
    }

    return QSharedPointer<IElanceJobsPage>(jobsPage);
}

QList<QSharedPointer<IElanceError> > ElanceDataReader::readErrors(const QByteArray & data)
{
    QList<QSharedPointer<IElanceError> > errors;

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonArray errorsArray = getErrorsArray(document);
    if (!errorsArray.isEmpty())
    {
        foreach (const QJsonValue & errorValue, errorsArray)
        {
            QSharedPointer<IElanceError> error = getError(errorValue);
            if (error->isValid())
            {
                errors.append(error);
            }
        }
    }

    return errors;
}

QJsonObject ElanceDataReader::getDataObject(const QJsonDocument & document)
{
    if (!document.isNull() && document.isObject())
    {
        QJsonValue dataValue = document.object()["data"];
        if (!dataValue.isUndefined() && dataValue.isObject())
        {
            return dataValue.toObject();
        }
    }
    return QJsonObject();
}

QSharedPointer<IElanceCategory> ElanceDataReader::getCategory(const QJsonValue & categoryValue)
{
    ElanceCategory * category = new ElanceCategory();

    if (categoryValue.isObject())
    {
        QJsonObject categoryObject = categoryValue.toObject();

        QJsonValue categoryIdValue = categoryObject["catId"];
        if (!categoryIdValue.isUndefined() && categoryIdValue.isDouble())
        {
            category->setCategoryId(categoryIdValue.toInt());
        }

        QJsonValue nameValue = categoryObject["catName"];
        if (!nameValue.isUndefined() && nameValue.isString())
        {
            category->setName(nameValue.toString());
        }

        QJsonValue childrenValue = categoryObject["children"];
        if (!childrenValue.isUndefined() && childrenValue.isObject())
        {
            foreach (const QJsonValue & subcategoryValue, childrenValue.toObject())
            {
                QSharedPointer<IElanceCategory> subcategory = getCategory(subcategoryValue);
                if (subcategory->isValid())
                {
                    category->addSubcategory(subcategory);
                }
            }
        }
    }

    return QSharedPointer<IElanceCategory>(category);
}

QSharedPointer<IElanceJob> ElanceDataReader::getJob(const QJsonValue & jobValue)
{
    ElanceJob * job = new ElanceJob();

    if (jobValue.isObject())
    {
        QJsonObject jobObject = jobValue.toObject();

        QJsonValue jobIdValue = jobObject["jobId"];
        if (!jobIdValue.isUndefined() && jobIdValue.isString())
        {
            bool isOk;
            int jobId = jobIdValue.toString().toInt(&isOk);
            if (isOk)
            {
                job->setJobId(jobId);
            }
        }

        QJsonValue nameValue = jobObject["name"];
        if (!nameValue.isUndefined() && nameValue.isString())
        {
            job->setName(nameValue.toString());
        }

        QJsonValue descriptionValue = jobObject["description"];
        if (!descriptionValue.isUndefined() && descriptionValue.isString())
        {
            job->setDescription(descriptionValue.toString());
        }

        QJsonValue budgetValue = jobObject["budget"];
        if (!budgetValue.isUndefined() && budgetValue.isString())
        {
            job->setBudget(budgetValue.toString());
        }

        QJsonValue isHourlyValue = jobObject["isHourly"];
        if (!isHourlyValue.isUndefined() && isHourlyValue.isDouble())
        {
            job->setIsHourly(isHourlyValue.toInt() == 1);
        }
    }

    return QSharedPointer<IElanceJob>(job);
}

QJsonArray ElanceDataReader::getErrorsArray(const QJsonDocument & document)
{
    if (!document.isNull() && document.isObject())
    {
        QJsonValue dataValue = document.object()["errors"];
        if (!dataValue.isUndefined() && dataValue.isArray())
        {
            return dataValue.toArray();
        }
    }
    return QJsonArray();
}

QSharedPointer<IElanceError> ElanceDataReader::getError(const QJsonValue & errorValue)
{
    ElanceError * error = new ElanceError();

    if (errorValue.isObject())
    {
        QJsonObject errorObject = errorValue.toObject();

        QJsonValue typeValue = errorObject["type"];
        if (!typeValue.isUndefined() && typeValue.isString())
        {
            error->setType(typeValue.toString());
        }

        QJsonValue codeValue = errorObject["code"];
        if (!codeValue.isUndefined() && codeValue.isString())
        {
            error->setCode(codeValue.toString());
        }

        QJsonValue descriptionValue = errorObject["description"];
        if (!descriptionValue.isUndefined() && descriptionValue.isString())
        {
            error->setDescription(descriptionValue.toString());
        }
    }

    return QSharedPointer<IElanceError>(error);
}
