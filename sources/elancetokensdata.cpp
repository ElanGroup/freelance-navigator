#include "elancetokensdata.h"

using namespace FreelanceNavigator;

ElanceTokensData::ElanceTokensData() : m_isNull(true)
{

}

ElanceTokensData::ElanceTokensData(const QString & accessToken, const QString & refreshToken)
    : m_isNull(false),
      m_accessToken(accessToken),
      m_refreshToken(refreshToken)
{
}

ElanceTokensData::~ElanceTokensData()
{
}

