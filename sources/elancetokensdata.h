#ifndef ELANCETOKENSDATA_H
#define ELANCETOKENSDATA_H

#include "ielancetokensdata.h"

namespace FreelanceNavigator
{
class ElanceTokensData : public IElanceTokensData
{
public:
    ElanceTokensData();
    ~ElanceTokensData();

    virtual QString accessToken() const;
    void setAccessToken(const QString & accessToken);
    virtual QString refreshToken() const;
    void setRefreshToken(const QString & refreshToken);

private:
    ElanceTokensData(const ElanceTokensData &);
    ElanceTokensData & operator=(const ElanceTokensData &);

    QString m_accessToken;
    QString m_refreshToken;
};
}

#endif // ELANCETOKENSDATA_H
