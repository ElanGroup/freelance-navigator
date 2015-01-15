#ifndef ELANCEDATAREADER_H
#define ELANCEDATAREADER_H

#include <QSharedPointer>

namespace FreelanceNavigator
{
class IElanceTokens;
class IElanceCategory;
class IElanceJobsPage;
class IElanceJob;

class ElanceDataReader
{
public:
    ElanceDataReader();
    ~ElanceDataReader();

    static QSharedPointer<IElanceTokens> readTokens(const QByteArray & data);
    static QList<QSharedPointer<IElanceCategory> > readCategories(const QByteArray & data);
    static QSharedPointer<IElanceJobsPage> readJobsPage(const QByteArray & data);

private:
    static QJsonObject getDataObject(const QJsonDocument & document);
    static QSharedPointer<IElanceCategory> getCategory(const QJsonValue & categoryValue);
    static QSharedPointer<IElanceJob> getJob(const QJsonValue & jobValue);
};
}

#endif // ELANCEDATAREADER_H
