#ifndef UPWORKENUMS_H
#define UPWORKENUMS_H

namespace FreelanceNavigator
{
namespace Upwork
{

enum class UpworkApiError
{
    ConnectionError,
    ServiceError
};

enum class UpworkApiWarning
{
    AuthorizationRequired
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKENUMS_H

