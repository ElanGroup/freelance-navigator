#ifndef ELANCETOKENS_H
#define ELANCETOKENS_H

#include "ielancetokens.h"

namespace FreelanceNavigator
{
class ElanceTokens : public IElanceTokens
{
public:
    ElanceTokens();
    ~ElanceTokens();

    virtual bool isValid() const;
    virtual QString accessToken() const;
    void setAccessToken(const QString & accessToken);
    virtual QString refreshToken() const;
    void setRefreshToken(const QString & refreshToken);

private:
    ElanceTokens(const ElanceTokens &);
    ElanceTokens & operator=(const ElanceTokens &);

    QString m_accessToken;
    QString m_refreshToken;
};
}

#endif // ELANCETOKENS_H
