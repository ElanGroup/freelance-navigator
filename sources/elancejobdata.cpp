#include "elancejobdata.h"

using namespace FreelanceNavigator;

ElanceJobData::ElanceJobData()
{
}

ElanceJobData::~ElanceJobData()
{
}

int ElanceJobData::jobId() const
{
    return m_jobId;
}

void ElanceJobData::setJobId(int jobId)
{
    m_jobId = jobId;
}
