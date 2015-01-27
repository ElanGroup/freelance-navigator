#include "elanceerror.h"

using namespace FreelanceNavigator;

ElanceError::ElanceError()
{
}

ElanceError::~ElanceError()
{
}

bool ElanceError::isValid() const
{
    return !m_code.isEmpty() && !m_description.isEmpty();
}

QString ElanceError::type() const
{
    return m_type;
}

void ElanceError::setType(const QString & type)
{
    m_type = type;
}

QString ElanceError::code() const
{
    return m_code;
}

void ElanceError::setCode(const QString & code)
{
    m_code = code;
}

QString ElanceError::description() const
{
    return m_description;
}

void ElanceError::setDescription(const QString & description)
{
    m_description = description;
}
