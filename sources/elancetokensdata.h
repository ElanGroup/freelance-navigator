#ifndef ELANCETOKENSDATA_H
#define ELANCETOKENSDATA_H

#include <QString>

namespace FreelanceNavigator
{
class ElanceTokensData
{
public:
    ElanceTokensData();
    ElanceTokensData(const QString & accessToken, const QString & refreshToken);
    ~ElanceTokensData();

    inline bool isNull() const { return m_isNull; }

    inline QString accessToken() const { return m_accessToken; }

    inline QString refreshToken() const { return m_refreshToken; }

private:
    bool m_isNull;
    QString m_accessToken;
    QString m_refreshToken;
};
}

#endif // ELANCETOKENSDATA_H
