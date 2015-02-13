#include "elancejob.h"

using namespace FreelanceNavigator;

ElanceJob::ElanceJob() : m_jobId(-1), m_isHourly(false)
{
}

ElanceJob::~ElanceJob()
{
}

bool ElanceJob::isValid() const
{
    return m_jobId != -1 &&
           !m_name.isEmpty() &&
           !m_description.isEmpty() &&
           !m_budget.isEmpty() &&
           !m_postedDate.isNull();
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

QString ElanceJob::budget() const
{
    return m_budget;
}

void ElanceJob::setBudget(const QString & budget)
{
    m_budget = budget;
}

const QDateTime & ElanceJob::postedDate() const
{
    return m_postedDate;
}

void ElanceJob::setPostedDate(const QDateTime & postedDate)
{
    m_postedDate = postedDate;
}

bool ElanceJob::isHourly() const
{
    return m_isHourly;
}

void ElanceJob::setIsHourly(bool isHourly)
{
    m_isHourly = isHourly;
}
