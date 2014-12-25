#ifndef ELANCEDATAREADER_H
#define ELANCEDATAREADER_H

#include <QSharedPointer>

namespace FreelanceNavigator
{
class IElanceTokensData;
class IElanceJobsData;

class ElanceDataReader
{
public:
    ElanceDataReader();
    ~ElanceDataReader();

    static QSharedPointer<IElanceTokensData> readTokensData(const QByteArray & data);
    static QSharedPointer<IElanceJobsData> readJobsData(const QByteArray & data);

private:
    static QJsonObject getDataObject(const QJsonDocument & document);
};
}

#endif // ELANCEDATAREADER_H
