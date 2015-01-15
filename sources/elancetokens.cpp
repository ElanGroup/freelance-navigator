#include "elancetokens.h"

using namespace FreelanceNavigator;

ElanceTokens::ElanceTokens()
{
}

ElanceTokens::~ElanceTokens()
{
}

bool ElanceTokens::isValid() const
{
    return !m_accessToken.isEmpty() && !m_refreshToken.isEmpty();
}

QString ElanceTokens::accessToken() const
{
    return m_accessToken;
}

void ElanceTokens::setAccessToken(const QString & accessToken)
{
    m_accessToken = accessToken;
}

QString ElanceTokens::refreshToken() const
{
    return m_refreshToken;
}

void ElanceTokens::setRefreshToken(const QString & refreshToken)
{
    m_refreshToken = refreshToken;
}
