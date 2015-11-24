#ifndef UPWORKERRORREADER_H
#define UPWORKERRORREADER_H

#include <QSharedPointer>

class QNetworkReply;

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkError;

class UpworkErrorReader
{
public:
    UpworkErrorReader();
    UpworkErrorReader(const UpworkErrorReader &) = delete;
    UpworkErrorReader & operator=(const UpworkErrorReader &) = delete;

    QSharedPointer<UpworkError> readError(QNetworkReply * reply) const;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKERRORREADER_H
