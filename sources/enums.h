#ifndef ENUMS_H
#define ENUMS_H

namespace FreelanceNavigator
{
namespace JobType
{
    enum Enum
    {
        Any,
        FixedPrice,
        Hourly
    };
}

namespace PostedDateRange
{
    enum Enum
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
}
}

#endif // ENUMS_H
