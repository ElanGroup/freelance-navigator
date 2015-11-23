#ifndef UPWORKERRORREADER_H
#define UPWORKERRORREADER_H

#include <QSharedPointer>

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

    QSharedPointer<UpworkError> readError(const QByteArray & data) const;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKERRORREADER_H
