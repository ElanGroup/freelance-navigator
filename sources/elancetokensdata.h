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

    inline virtual bool isNull() const { return m_isNull; }

    inline virtual QString accessToken() const { return m_accessToken; }

    inline virtual QString refreshToken() const { return m_refreshToken; }

    void setAccessToken(const QString & accessToken);
    void setRefreshToken(const QString & refreshToken);

private:
    ElanceTokensData(const ElanceTokensData &);
    ElanceTokensData & operator=(const ElanceTokensData &);

    bool m_isNull;
    QString m_accessToken;
    QString m_refreshToken;
};
}

#endif // ELANCETOKENSDATA_H
