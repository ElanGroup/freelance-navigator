#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "elancedatareader.h"
#include "ielancetokensdata.h"
#include "elancetokensdata.h"
#include "ielancejobsdata.h"
#include "elancejobsdata.h"
#include "elancejobdata.h"

using namespace FreelanceNavigator;

ElanceDataReader::ElanceDataReader()
{
}

ElanceDataReader::~ElanceDataReader()
{
}

QSharedPointer<IElanceTokensData> ElanceDataReader::readTokensData(const QByteArray & data)
{
    ElanceTokensData * tokensData = new ElanceTokensData();

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject dataObject = getDataObject(document);
    if (!dataObject.isEmpty())
    {
        tokensData->setIsValid(true);

        QJsonValue accessToken = dataObject["access_token"];
        if (!accessToken.isUndefined() && accessToken.isString())
        {
            tokensData->setAccessToken(accessToken.toString());
        }

        QJsonValue refreshToken = dataObject["refresh_token"];
        if (!refreshToken.isUndefined() && refreshToken.isString())
        {
            tokensData->setRefreshToken(refreshToken.toString());
        }
    }

    return QSharedPointer<IElanceTokensData>(tokensData);
}

QSharedPointer<IElanceJobsData> ElanceDataReader::readJobsData(const QByteArray & data)
{
    ElanceJobsData * jobsData = new ElanceJobsData();

    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject dataObject = getDataObject(document);
    if (!dataObject.isEmpty())
    {
        jobsData->setIsValid(true);

        QJsonValue totalResults = dataObject["totalResults"];
        if (!totalResults.isUndefined() && totalResults.isDouble())
        {
            jobsData->setJobsTotal(totalResults.toInt());
        }

        QJsonValue pageResults = dataObject["pageResults"];
        if (!pageResults.isUndefined() && pageResults.isArray())
        {
            QJsonArray jobsArray = pageResults.toArray();
            foreach (const QJsonValue & jobValue, jobsArray)
            {
                jobsData->addJob(getJobData(jobValue));
            }
        }
    }

    return QSharedPointer<IElanceJobsData>(jobsData);
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

ElanceJobData * ElanceDataReader::getJobData(const QJsonValue & jobValue)
{
    ElanceJobData * jobData = new ElanceJobData();

    if (jobValue.isObject())
    {
        QJsonObject jobObject = jobValue.toObject();

        QJsonValue jobIdValue = jobObject["jobId"];
        if (!jobIdValue.isUndefined() && jobIdValue.isString())
        {
            jobData->setJobId(jobIdValue.toString().toInt());
        }
    }

    return jobData;
}
