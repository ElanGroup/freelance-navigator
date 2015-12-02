#ifndef UPWORKENUMS_H
#define UPWORKENUMS_H

namespace FreelanceNavigator
{
namespace Upwork
{

enum class UpworkApiError
{
    ConnectionError,
    AuthenticationError,
    ServiceError
};

enum class UpworkApiWarning
{
    AuthorizationRequired
};

enum class PostedDateRange
{
    Any,
    Day,
    ThreeDays,
    FiveDays,
    Week,
    TenDays,
    TwoWeeks,
    Month
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKENUMS_H

