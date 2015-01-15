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
    return m_jobId != -1;
}

int ElanceJob::jobId() const
{
    return m_jobId;
}

void ElanceJob::setJobId(int jobId)
{
    m_jobId = jobId;
}
