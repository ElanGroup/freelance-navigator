#ifndef IELANCETOKENS_H
#define IELANCETOKENS_H

#include <QString>
#include "ielancedata.h"

namespace FreelanceNavigator
{
class IElanceTokens : public IElanceData
{
public:    
    virtual ~IElanceTokens() {}
    virtual QString accessToken() const = 0;
    virtual QString refreshToken() const = 0;
};
}

#endif // IELANCETOKENS_H
