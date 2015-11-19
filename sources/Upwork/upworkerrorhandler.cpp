#include <QDebug>
#include <QNetworkReply>
#include "upworkerrorhandler.h"

using namespace FreelanceNavigator::Upwork;

UpworkErrorHandler::UpworkErrorHandler(QNetworkReply * reply) : m_reply(reply)
{
}

bool UpworkErrorHandler::hasErrors() const
{
    return m_reply->error() != QNetworkReply::NoError;
}
