#ifndef UPWORKTOKENREADER_H
#define UPWORKTOKENREADER_H

#include <QString>

class QNetworkReply;
class QUrlQuery;

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkTokenReader
{
public:
    UpworkTokenReader(QNetworkReply * reply);
    ~UpworkTokenReader();
    UpworkTokenReader(const UpworkTokenReader &) = delete;
    UpworkTokenReader & operator=(const UpworkTokenReader &) = delete;

    QString readToken();
    QString readTokenSecret();

private:
    void readData();

    QNetworkReply * m_reply;
    QUrlQuery * m_data;

    static const QString m_requestTokenName;
    static const QString m_requestTokenSecretName;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKTOKENREADER_H
