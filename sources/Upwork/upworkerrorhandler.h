#ifndef UPWORKERRORHANDLER_H
#define UPWORKERRORHANDLER_H

class QNetworkReply;

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkErrorHandler
{
public:
    UpworkErrorHandler(QNetworkReply * reply);
    UpworkErrorHandler(const UpworkErrorHandler &) = delete;
    UpworkErrorHandler & operator=(const UpworkErrorHandler &) = delete;

    bool hasErrors() const;

private:
    QNetworkReply * m_reply;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKERRORHANDLER_H
