#include <QJsonDocument>
#include <QJsonObject>
#include "elancedatareader.h"
#include "ielancetokensdata.h"
#include "elancetokensdata.h"

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
    if (!document.isNull())
    {
        QJsonValue dataValue = document.object()["data"];
        if (!dataValue.isUndefined() && dataValue.isObject())
        {
            QJsonObject dataObject = dataValue.toObject();

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
    }

    return QSharedPointer<IElanceTokensData>(tokensData);
}
