#include <QNetworkReply>
#include <QUrlQuery>
#include "upworktokenreader.h"

using namespace FreelanceNavigator::Upwork;

const QString UpworkTokenReader::m_requestTokenName("oauth_token");
const QString UpworkTokenReader::m_requestTokenSecretName("oauth_token_secret");

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

QString UpworkTokenReader::readToken()
{
    if (!m_data)
    {
        readData();
    }
    return m_data->queryItemValue(m_requestTokenName);
}

QString UpworkTokenReader::readTokenSecret()
{
    if (!m_data)
    {
        readData();
    }
    return m_data->queryItemValue(m_requestTokenSecretName);
}

void UpworkTokenReader::readData()
{
    m_data = new QUrlQuery(m_reply->readAll());
}
