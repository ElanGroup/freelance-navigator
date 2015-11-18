#ifndef SETTINGS_H
#define SETTINGS_H

#include "Upwork/upworksettings.h"

namespace FreelanceNavigator
{

class Settings : public FreelanceNavigator::Upwork::UpworkSettings
{
public:
    Settings();
    Settings(const Settings &) = delete;
    Settings & operator=(const Settings &) = delete;

    virtual QString upworkKey() override;
    virtual QString upworkSecret() override;

private:
    void readUpworkKeys();

    QString m_upworkKey;
    QString m_upworkSecret;
};

} // namespace FreelanceNavigator

#endif // SETTINGS_H
