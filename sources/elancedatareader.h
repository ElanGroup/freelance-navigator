#ifndef ELANCEDATAREADER_H
#define ELANCEDATAREADER_H

#include <QSharedPointer>

namespace FreelanceNavigator
{
class IElanceTokensData;

class ElanceDataReader
{
public:
    ElanceDataReader();
    ~ElanceDataReader();

    static QSharedPointer<IElanceTokensData> readTokensData(const QByteArray & data);
};
}

#endif // ELANCEDATAREADER_H
