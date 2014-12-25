#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "elancedatareader.h"
#include "ielancetokensdata.h"
#include "elancetokensdata.h"
#include "ielancejobsdata.h"
#include "elancejobsdata.h"

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
            jobsData->setJobsCount(totalResults.toInt());
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
