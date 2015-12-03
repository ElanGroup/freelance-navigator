#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include "upworkjobreader.h"
#include "upworkjobpage.h"
#include "upworkjob.h"

using namespace FreelanceNavigator::Upwork;

UpworkJobReader::UpworkJobReader()
{
}

std::unique_ptr<UpworkJobPage> UpworkJobReader::readJobPage(QNetworkReply * reply) const
{
    UpworkJobPage * jobPage = new UpworkJobPage();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if (!document.isNull() && document.isObject())
    {
        QJsonObject jobPageObject = document.object();
        getJobs(jobPage, jobPageObject);
        getPagingData(jobPage, jobPageObject);
    }
    return std::unique_ptr<UpworkJobPage>(jobPage);
}

void UpworkJobReader::getJobs(UpworkJobPage * const jobPage, const QJsonObject & jobPageObject)
{
    QJsonValue jobsValue = jobPageObject["jobs"];
    if (jobsValue.isArray())
    {
        foreach (const QJsonValue & jobValue, jobsValue.toArray())
        {
            UpworkJob * job = getJob(jobValue);
            if (checkIsValidJob(job))
            {
                jobPage->addJob(QSharedPointer<UpworkJob>(job));
            }
        }
    }
}

UpworkJob * UpworkJobReader::getJob(const QJsonValue & jobValue)
{
    UpworkJob * job = new UpworkJob();
    if (jobValue.isObject())
    {
        QJsonObject jobObject = jobValue.toObject();

        QJsonValue idValue = jobObject["id"];
        if (idValue.isString())
        {
            job->setJobId(idValue.toString());
        }

        QJsonValue titleValue = jobObject["title"];
        if (titleValue.isString())
        {
            job->setTitle(titleValue.toString());
        }

        QJsonValue descriptionValue = jobObject["snippet"];
        if (descriptionValue.isString())
        {
            job->setDescription(descriptionValue.toString());
        }

        QJsonValue typeValue = jobObject["job_type"];
        if (typeValue.isString())
        {
            job->setType(typeValue.toString() == "Fixed" ? JobType::Fixed : JobType::Hourly);
        }

        QJsonValue budgetValue = jobObject["budget"];
        if (budgetValue.isDouble())
        {
            job->setBudget(budgetValue.toInt());
        }

        QJsonValue dateCreatedValue = jobObject["date_created"];
        if (dateCreatedValue.isString())
        {
            job->setPostedDate(QDateTime::fromString(dateCreatedValue.toString(), Qt::ISODate));
        }

        QJsonValue skillsValue = jobObject["skills"];
        if (skillsValue.isArray())
        {
            foreach (const QJsonValue & skillValue, skillsValue.toArray())
            {
                if (skillValue.isString())
                {
                    job->addSkill(skillValue.toString());
                }
            }
        }

        QJsonValue urlValue = jobObject["url"];
        if (urlValue.isString())
        {
            job->setUrl(urlValue.toString());
        }
    }
    return job;
}

bool UpworkJobReader::checkIsValidJob(const UpworkJob * const job)
{
    return !job->jobId().isEmpty() &&
           !job->title().isEmpty() &&
           !job->description().isEmpty() &&
           !job->url().isEmpty();
}

void UpworkJobReader::getPagingData(UpworkJobPage * const jobPage,
                                    const QJsonObject & jobPageObject)
{
    QJsonValue pagingValue = jobPageObject["paging"];
    if (pagingValue.isObject())
    {
        QJsonObject pagingObject = pagingValue.toObject();

        QJsonValue offsetValue = pagingObject["offset"];
        if (offsetValue.isDouble())
        {
            jobPage->setOffset(offsetValue.toInt());
        }

        QJsonValue countValue = pagingObject["count"];
        if (countValue.isDouble())
        {
            jobPage->setCount(countValue.toInt());
        }

        QJsonValue totalValue = pagingObject["total"];
        if (totalValue.isDouble())
        {
            jobPage->setTotal(totalValue.toInt());
        }
    }
}
