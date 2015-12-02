#include <QSettings>
#include "settings.h"
#include "Upwork/upworksearchjobparameters.h"

using namespace FreelanceNavigator;
using namespace FreelanceNavigator::Upwork;

const QString Settings::m_upworkApiGroupName("Upwork API");
const QString Settings::m_upworkAccessTokenName("Access Token");
const QString Settings::m_upworkAccessTokenSecretName("Access Token Secret");
const QString Settings::m_upworkSettingsGroupName("Upwork");
const QString Settings::m_upworkCategoryName("Category");
const QString Settings::m_upworkSubcategoriesName("Subcategories");
const QString Settings::m_upworkPostedDateRangeName("Posted Date");
const QString Settings::m_upworkJobTypeName("Job Type");
const QString Settings::m_upworkMinBudgetName("Min Budget");
const QString Settings::m_upworkMaxBudgetName("Max Budget");

Settings::Settings()
{
}

QString Settings::upworkKey()
{
    readUpworkKeys();
    return m_upworkKey;
}

QString Settings::upworkSecret()
{
    readUpworkKeys();
    return m_upworkSecret;
}

QString Settings::upworkAccessToken()
{
    readUpworkAccessToken();
    return m_upworkAccessToken;
}

QString Settings::upworkAccessTokenSecret()
{
    readUpworkAccessToken();
    return m_upworkAccessTokenSecret;
}

void Settings::saveUpworkAccessToken(const QString & accessToken,
                                     const QString & accessTokenSecret)
{
    QSettings settings;
    settings.beginGroup(m_upworkApiGroupName);
    settings.setValue(m_upworkAccessTokenName, accessToken);
    settings.setValue(m_upworkAccessTokenSecretName, accessTokenSecret);

    m_upworkAccessToken = accessToken;
    m_upworkAccessTokenSecret = accessTokenSecret;
}

void Settings::readUpworkKeys()
{
    if (m_upworkKeysWereRead)
    {
        return;
    }

    QSettings settings(QStringLiteral(":/Resources/upwork-api.ini"), QSettings::IniFormat);
    m_upworkKey = settings.value(QStringLiteral("Key")).toString();
    m_upworkSecret = settings.value(QStringLiteral("Secret")).toString();

    m_upworkKeysWereRead = true;
}

void Settings::readUpworkAccessToken()
{
    if (m_upworkAccessTokenWasRead)
    {
        return;
    }

    QSettings settings;
    settings.beginGroup(m_upworkApiGroupName);

    QVariant accessToken = settings.value(m_upworkAccessTokenName);
    if (accessToken.isValid())
    {
        m_upworkAccessToken = accessToken.toString();
    }

    QVariant accessTokenSecret = settings.value(m_upworkAccessTokenSecretName);
    if (accessTokenSecret.isValid())
    {
        m_upworkAccessTokenSecret = accessTokenSecret.toString();
    }

    m_upworkAccessTokenWasRead = true;
}

void Settings::removeUpworkAccessToken()
{
    QSettings settings;
    settings.beginGroup(m_upworkApiGroupName);
    settings.remove(m_upworkAccessTokenName);
    settings.remove(m_upworkAccessTokenSecretName);
    m_upworkAccessToken.clear();
    m_upworkAccessTokenSecret.clear();
}

QString Settings::upworkCategory()
{
    readUpworkSettings();
    return m_upworkCategory;
}

QStringList Settings::upworkSubcategories()
{
    readUpworkSettings();
    return m_upworkSubcategories;
}

int Settings::upworkPostedDateRange()
{
    readUpworkSettings();
    return m_upworkPostedDateRange;
}

int Settings::upworkJobType()
{
    readUpworkSettings();
    return m_upworkJobType;
}

int Settings::upworkMinBudget()
{
    readUpworkSettings();
    return m_upworkMinBudget;
}

int Settings::upworkMaxBudget()
{
    readUpworkSettings();
    return m_upworkMaxBudget;
}

void Settings::readUpworkSettings()
{
    if (m_upworkSettingsWereRead)
    {
        return;
    }

    QSettings settings;
    settings.beginGroup(m_upworkSettingsGroupName);

    QVariant category = settings.value(m_upworkCategoryName);
    if (category.isValid())
    {
        m_upworkCategory = category.toString();
    }

    QVariant subcategories = settings.value(m_upworkSubcategoriesName);
    if (subcategories.isValid())
    {
        m_upworkSubcategories = subcategories.toStringList();
    }

    QVariant postedDateRange = settings.value(m_upworkPostedDateRangeName);
    if (postedDateRange.isValid())
    {
        m_upworkPostedDateRange = postedDateRange.toInt();
    }

    QVariant jobType = settings.value(m_upworkJobTypeName);
    if (jobType.isValid())
    {
        m_upworkJobType = jobType.toInt();
    }

    QVariant minBudget = settings.value(m_upworkMinBudgetName);
    if (minBudget.isValid())
    {
        m_upworkMinBudget = minBudget.toInt();
    }

    QVariant maxBudget = settings.value(m_upworkMaxBudgetName);
    if (maxBudget.isValid())
    {
        m_upworkMaxBudget = maxBudget.toInt();
    }

    m_upworkSettingsWereRead = true;
}

void Settings::saveUpworkSettings(const UpworkSearchJobParameters & parameters)
{
    QSettings settings;
    settings.beginGroup(m_upworkSettingsGroupName);
    settings.setValue(m_upworkCategoryName, parameters.category());
    settings.setValue(m_upworkSubcategoriesName, parameters.subcategories());
    settings.setValue(m_upworkPostedDateRangeName, static_cast<int>(parameters.postedDateRange()));
    settings.setValue(m_upworkJobTypeName, static_cast<int>(parameters.jobType()));
    if (parameters.minBudget() == -1)
    {
        settings.remove(m_upworkMinBudgetName);
    }
    else
    {
        settings.setValue(m_upworkMinBudgetName, parameters.minBudget());
    }
    if (parameters.maxBudget() == -1)
    {
        settings.remove(m_upworkMaxBudgetName);
    }
    else
    {
        settings.setValue(m_upworkMaxBudgetName, parameters.maxBudget());
    }

    m_upworkCategory = parameters.category();
    m_upworkSubcategories = parameters.subcategories();
    m_upworkPostedDateRange = static_cast<int>(parameters.postedDateRange());
    m_upworkJobType = static_cast<int>(parameters.jobType());
    m_upworkMinBudget = parameters.minBudget();
    m_upworkMaxBudget = parameters.maxBudget();
}
