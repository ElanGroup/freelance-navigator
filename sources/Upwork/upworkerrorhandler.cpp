#include <QDebug>
#include "upworkerrorhandler.h"
#include "apiresponse.h"

using namespace FreelanceNavigator::Upwork;

UpworkErrorHandler::UpworkErrorHandler()
{

}

void UpworkErrorHandler::handleError(const ApiResponse * const response)
{
    qDebug() << response->networkError() << response->data();
}
