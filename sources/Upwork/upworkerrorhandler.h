#ifndef UPWORKERRORHANDLER_H
#define UPWORKERRORHANDLER_H

namespace FreelanceNavigator
{

class ApiResponse;

namespace Upwork
{

class UpworkErrorHandler
{
public:
    UpworkErrorHandler();
    UpworkErrorHandler(const UpworkErrorHandler &) = delete;
    UpworkErrorHandler & operator=(const UpworkErrorHandler &) = delete;

    void handleError(const ApiResponse * const response);
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKERRORHANDLER_H
