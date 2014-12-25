#include "elancetokensdata.h"

using namespace FreelanceNavigator;

ElanceTokensData::ElanceTokensData()
{
}

ElanceTokensData::~ElanceTokensData()
{
}

void ElanceTokensData::setAccessToken(const QString & accessToken)
{
    m_accessToken = accessToken;
}

void ElanceTokensData::setRefreshToken(const QString & refreshToken)
{
    m_refreshToken = refreshToken;
}
