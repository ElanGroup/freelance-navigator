#include <QJsonDocument>
#include <QJsonObject>
#include "elancedatareader.h"
#include "elancetokensdata.h"

using namespace FreelanceNavigator;

ElanceDataReader::ElanceDataReader()
{
}

ElanceDataReader::~ElanceDataReader()
{
}

QSharedPointer<ElanceTokensData> ElanceDataReader::readTokensData(const QByteArray & data)
{
    QJsonDocument document = QJsonDocument::fromJson(data);
    if (document.isNull())
    {
        return QSharedPointer<ElanceTokensData>(new ElanceTokensData());
    }

    QJsonValue dataValue = document.object()["data"];
    if (dataValue.isUndefined() || !dataValue.isObject())
    {
        return QSharedPointer<ElanceTokensData>(new ElanceTokensData());
    }

    QJsonObject dataObject = dataValue.toObject();
    QJsonValue accessToken = dataObject["access_token"];
    QJsonValue refreshToken = dataObject["refresh_token"];
    if (accessToken.isUndefined() ||
        !accessToken.isString() ||
        refreshToken.isUndefined() ||
        !refreshToken.isString())
    {
        return QSharedPointer<ElanceTokensData>(new ElanceTokensData());
    }

    return QSharedPointer<ElanceTokensData>(new ElanceTokensData(accessToken.toString(),
                                                                 refreshToken.toString()));
}
