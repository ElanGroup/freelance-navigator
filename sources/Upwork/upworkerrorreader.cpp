#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "upworkerrorreader.h"
#include "upworkerror.h"

using namespace FreelanceNavigator::Upwork;

UpworkErrorReader::UpworkErrorReader()
{
}

QSharedPointer<UpworkError> UpworkErrorReader::readError(QNetworkReply * reply) const
{
    UpworkError * error = new UpworkError();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if (!document.isNull() && document.isObject())
    {
        QJsonValue errorValue = document.object()["error"];
        if (!errorValue.isUndefined() && errorValue.isObject())
        {
            QJsonObject errorObject = errorValue.toObject();

            QJsonValue statusValue = errorObject["status"];
            if (!statusValue.isUndefined() && statusValue.isDouble())
            {
                error->setStatus(statusValue.toInt());
            }

            QJsonValue codeValue = errorObject["code"];
            if (!codeValue.isUndefined() && codeValue.isDouble())
            {
                error->setCode(codeValue.toInt());
            }

            QJsonValue messageValue = errorObject["message"];
            if (!messageValue.isUndefined() && messageValue.isString())
            {
                error->setMessage(messageValue.toString());
            }
        }
    }
    return QSharedPointer<UpworkError>(error);
}
