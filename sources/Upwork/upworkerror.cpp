#include "upworkerror.h"

using namespace FreelanceNavigator::Upwork;

UpworkError::UpworkError() : m_status(-1), m_code(-1)
{
}

int UpworkError::status() const
{
    return m_status;
}

void UpworkError::setStatus(int status)
{
    m_status = status;
}

int UpworkError::code() const
{
    return m_code;
}

void UpworkError::setCode(int code)
{
    m_code = code;
}

QString UpworkError::message() const
{
    return m_message;
}

void UpworkError::setMessage(const QString & message)
{
    m_message = message;
}
