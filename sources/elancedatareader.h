#ifndef ELANCEDATAREADER_H
#define ELANCEDATAREADER_H

#include <QSharedPointer>

namespace FreelanceNavigator
{
class ElanceTokensData;

class ElanceDataReader
{
public:
    ElanceDataReader();
    ~ElanceDataReader();

    static QSharedPointer<ElanceTokensData> readTokensData(const QByteArray & data);
};
}

#endif // ELANCEDATAREADER_H
