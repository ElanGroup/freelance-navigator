#ifndef IELANCETOKENSDATA_H
#define IELANCETOKENSDATA_H

#include <QString>
#include "ielancedata.h"

namespace FreelanceNavigator
{
class IElanceTokensData : public IElanceData
{
public:    
    virtual ~IElanceTokensData() {}
    virtual QString accessToken() const = 0;
    virtual QString refreshToken() const = 0;
};
}

#endif // IELANCETOKENSDATA_H
