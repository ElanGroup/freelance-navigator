#ifndef UPWORKERROR_H
#define UPWORKERROR_H

#include <QString>

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkError
{
public:
    UpworkError();
    UpworkError(const UpworkError &) = delete;
    UpworkError & operator=(const UpworkError &) = delete;

    int status() const;
    void setStatus(int status);
    int code() const;
    void setCode(int code);
    QString message() const;
    void setMessage(const QString & message);

private:
    int m_status;
    int m_code;
    QString m_message;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKERROR_H
