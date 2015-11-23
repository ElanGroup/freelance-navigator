#include <QNetworkReply>
#include <QUrlQuery>
#include "upworktokenreader.h"

using namespace FreelanceNavigator::Upwork;

const QString UpworkTokenReader::m_requestTokenName("oauth_token");
const QString UpworkTokenReader::m_requestTokenSecretName("oauth_token_secret");
const QString UpworkTokenReader::m_accessTokenName("oauth_access_token");
const QString UpworkTokenReader::m_accessTokenSecretName("oauth_access_token_secret");

UpworkTokenReader::UpworkTokenReader(QNetworkReply * reply) : m_reply(reply), m_data(nullptr)
{
}

UpworkTokenReader::~UpworkTokenReader()
{
    if (m_data)
    {
        delete m_data;
    }
}

QString UpworkTokenReader::readRequestToken()
{
    if (!m_data)
    {
        readData();
    }
    return m_data->queryItemValue(m_requestTokenName);
}

QString UpworkTokenReader::readRequestTokenSecret()
{
    if (!m_data)
    {
        readData();
    }
    return m_data->queryItemValue(m_requestTokenSecretName);
}

QString UpworkTokenReader::readAccessToken()
{
    if (!m_data)
    {
        readData();
    }
    return m_data->queryItemValue(m_accessTokenName);
}

QString UpworkTokenReader::readAccessTokenSecret()
{
    if (!m_data)
    {
        readData();
    }
    return m_data->queryItemValue(m_accessTokenSecretName);
}

void UpworkTokenReader::readData()
{
    m_data = new QUrlQuery(m_reply->readAll());
}
