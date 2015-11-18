#ifndef UPWORKSETTINGS_H
#define UPWORKSETTINGS_H

#include <QString>

namespace FreelanceNavigator
{
namespace Upwork
{

class UpworkSettings
{
public:
    virtual ~UpworkSettings() {}
    virtual QString upworkKey() = 0;
    virtual QString upworkSecret() = 0;
};

} // namespace Upwork
} // namespace FreelanceNavigator

#endif // UPWORKSETTINGS_H
