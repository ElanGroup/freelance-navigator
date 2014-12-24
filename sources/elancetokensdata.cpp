#include "elancetokensdata.h"

using namespace FreelanceNavigator;

ElanceTokensData::ElanceTokensData() : m_isNull(true)
{
}

ElanceTokensData::~ElanceTokensData()
{
}

void ElanceTokensData::setAccessToken(const QString & accessToken)
{
    m_isNull = false;
    m_accessToken = accessToken;
}

void ElanceTokensData::setRefreshToken(const QString & refreshToken)
{
    m_isNull = false;
    m_refreshToken = refreshToken;
}
