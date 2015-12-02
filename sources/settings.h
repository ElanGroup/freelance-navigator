#ifndef SETTINGS_H
#define SETTINGS_H

#include "Upwork/upworksettings.h"

namespace FreelanceNavigator
{

namespace Upwork
{
class UpworkSearchJobParameters;
}

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
    virtual void removeUpworkAccessToken() override;

    QString upworkCategory();
    QStringList upworkSubcategories();
    int upworkPostedDateRange();
    int upworkJobType();
    void saveUpworkSettings(const Upwork::UpworkSearchJobParameters & parameters);

private:
    void readUpworkKeys();
    void readUpworkAccessToken();
    void readUpworkSettings();

    bool m_upworkKeysWereRead = false;
    bool m_upworkAccessTokenWasRead = false;
    bool m_upworkSettingsWereRead = false;

    QString m_upworkKey;
    QString m_upworkSecret;
    QString m_upworkAccessToken;
    QString m_upworkAccessTokenSecret;
    QString m_upworkCategory;
    QStringList m_upworkSubcategories;
    int m_upworkPostedDateRange = 0;
    int m_upworkJobType = 0;

    static const QString m_upworkApiGroupName;
    static const QString m_upworkAccessTokenName;
    static const QString m_upworkAccessTokenSecretName;
    static const QString m_upworkSettingsGroupName;
    static const QString m_upworkCategoryName;
    static const QString m_upworkSubcategoriesName;
    static const QString m_upworkPostedDateRangeName;
    static const QString m_upworkJobTypeName;
};

} // namespace FreelanceNavigator

#endif // SETTINGS_H
