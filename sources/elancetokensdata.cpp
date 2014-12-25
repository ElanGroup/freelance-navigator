#include "elancetokensdata.h"

using namespace FreelanceNavigator;

ElanceTokensData::ElanceTokensData()
{
}

ElanceTokensData::~ElanceTokensData()
{
}

QString ElanceTokensData::accessToken() const
{
    return m_accessToken;
}

void ElanceTokensData::setAccessToken(const QString & accessToken)
{
    m_accessToken = accessToken;
}

QString ElanceTokensData::refreshToken() const
{
    return m_refreshToken;
}

void ElanceTokensData::setRefreshToken(const QString & refreshToken)
{
    m_refreshToken = refreshToken;
}
