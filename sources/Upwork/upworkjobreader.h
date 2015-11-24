#ifndef UPWORKJOBREADER_H
#define UPWORKJOBREADER_H

#include <memory>
#include <QSharedPointer>

class QNetworkReply;

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkJob;
class UpworkJobPage;

class UpworkJobReader
{
public:
    UpworkJobReader();
    UpworkJobReader(const UpworkJobReader &) = delete;
    UpworkJobReader & operator=(const UpworkJobReader &) = delete;

    std::unique_ptr<UpworkJobPage> readJobPage(QNetworkReply * reply) const;

private:
    QSharedPointer<UpworkJob> getJob(const QJsonValue & jobValue) const;
    static bool checkIsValidJob(const UpworkJob & job);
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKJOBREADER_H
