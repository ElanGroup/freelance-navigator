#ifndef IELANCEERROR
#define IELANCEERROR

#include <QString>
#include "ielancedata.h"

namespace FreelanceNavigator
{
class IElanceError : public IElanceData
{
public:
    virtual ~IElanceError() {}
    virtual QString type() const = 0;
    virtual QString code() const = 0;
    virtual QString description() const = 0;
};
}

#endif // IELANCEERROR

