#ifndef IELANCEDATA_H
#define IELANCEDATA_H

namespace FreelanceNavigator
{
class IElanceData
{
public:
    virtual ~IElanceData() {}
    virtual bool isNull() const = 0;
};
}

#endif // IELANCEDATA_H
