#include "elancejob.h"

using namespace FreelanceNavigator;

ElanceJob::ElanceJob() : m_jobId(-1)
{
}

ElanceJob::~ElanceJob()
{
}

bool ElanceJob::isValid() const
{
    return m_jobId != -1 && !m_name.isEmpty() && !m_description.isEmpty();
}

int ElanceJob::jobId() const
{
    return m_jobId;
}

void ElanceJob::setJobId(int jobId)
{
    m_jobId = jobId;
}

QString ElanceJob::name() const
{
    return m_name;
}

void ElanceJob::setName(const QString & name)
{
    m_name = name;
}

QString ElanceJob::description() const
{
    return m_description;
}

void ElanceJob::setDescription(const QString & description)
{
    m_description = description;
}