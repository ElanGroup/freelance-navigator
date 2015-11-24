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
    auto jobPage = std::make_unique<UpworkJobPage>();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if (!document.isNull() && document.isObject())
    {
        QJsonObject jobPageObject = document.object();

        QJsonValue jobsValue = jobPageObject["jobs"];
        if (!jobsValue.isUndefined() && jobsValue.isArray())
        {
            foreach (const QJsonValue & jobValue, jobsValue.toArray())
            {
                QSharedPointer<UpworkJob> job = getJob(jobValue);
                if (checkIsValidJob(*job))
                {
                    jobPage->addJob(job);
                }
            }
        }

        QJsonValue pagingValue = jobPageObject["paging"];
        if (!pagingValue.isUndefined() && pagingValue.isObject())
        {
            QJsonObject pagingObject = pagingValue.toObject();

            QJsonValue offsetValue = pagingObject["offset"];
            if (!offsetValue.isUndefined() && offsetValue.isDouble())
            {
                jobPage->setOffset(offsetValue.toInt());
            }

            QJsonValue countValue = pagingObject["count"];
            if (!countValue.isUndefined() && countValue.isDouble())
            {
                jobPage->setCount(countValue.toInt());
            }

            QJsonValue totalValue = pagingObject["total"];
            if (!totalValue.isUndefined() && totalValue.isDouble())
            {
                jobPage->setTotal(totalValue.toInt());
            }
        }
    }
    return jobPage;
}

QSharedPointer<UpworkJob> UpworkJobReader::getJob(const QJsonValue & jobValue) const
{
    UpworkJob * job = new UpworkJob();
    if (jobValue.isObject())
    {
        QJsonObject jobObject = jobValue.toObject();

        QJsonValue idValue = jobObject["id"];
        if (!idValue.isUndefined() && idValue.isString())
        {
            job->setJobId(idValue.toString());
        }

        QJsonValue titleValue = jobObject["title"];
        if (!titleValue.isUndefined() && titleValue.isString())
        {
            job->setTitle(titleValue.toString());
        }

        QJsonValue descriptionValue = jobObject["snippet"];
        if (!descriptionValue.isUndefined() && descriptionValue.isString())
        {
            job->setDescription(descriptionValue.toString());
        }
    }
    return QSharedPointer<UpworkJob>(job);
}

bool UpworkJobReader::checkIsValidJob(const UpworkJob & job)
{
    return !job.jobId().isEmpty() && !job.title().isEmpty() && !job.description().isEmpty();
}
