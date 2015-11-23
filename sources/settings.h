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
    virtual QString upworkAccessToken() override;
    virtual QString upworkAccessTokenSecret() override;
    virtual void saveUpworkAccessToken(const QString & accessToken,
                                       const QString & accessTokenSecret) override;

private:
    void readUpworkKeys();
    void readUpworkAccessToken();

    QString m_upworkKey;
    QString m_upworkSecret;
    QString m_upworkAccessToken;
    QString m_upworkAccessTokenSecret;

    static const QString m_upworkApiGroupName;
    static const QString m_upworkAccessTokenName;
    static const QString m_upworkAccessTokenSecretName;
};

} // namespace FreelanceNavigator

#endif // SETTINGS_H
