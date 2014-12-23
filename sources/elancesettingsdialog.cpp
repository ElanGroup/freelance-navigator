#include <QPushButton>
#include <QSettings>
#include "elancesettingsdialog.h"
#include "ui_elancesettingsdialog.h"

using namespace FreelanceNavigator;

ElanceSettingsDialog::ElanceSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ElanceSettingsDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setupConnections();
    loadSettings();
    validateSettings();
}

ElanceSettingsDialog::~ElanceSettingsDialog()
{
    delete ui;
}

void ElanceSettingsDialog::validateSettings()
{
    bool validationResult = !ui->apiKeyLineEdit->text().isEmpty() &&
                            !ui->secretCodeLineEdit->text().isEmpty() &&
                            !ui->redirectUriLineEdit->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(validationResult);
}

void ElanceSettingsDialog::saveSettings()
{
    QSettings settings;
    settings.beginGroup("Elance API");
    settings.setValue("API Key", ui->apiKeyLineEdit->text());
    settings.setValue("Secret Code", ui->secretCodeLineEdit->text());
    settings.setValue("Redirect URI", ui->redirectUriLineEdit->text());

    accept();
}

void ElanceSettingsDialog::setupConnections()
{
    connect(ui->apiKeyLineEdit, &QLineEdit::textChanged,
            this, &ElanceSettingsDialog::validateSettings);
    connect(ui->secretCodeLineEdit, &QLineEdit::textChanged,
            this, &ElanceSettingsDialog::validateSettings);
    connect(ui->redirectUriLineEdit, &QLineEdit::textChanged,
            this, &ElanceSettingsDialog::validateSettings);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ElanceSettingsDialog::saveSettings);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void ElanceSettingsDialog::loadSettings()
{
    QSettings settings;
    settings.beginGroup("Elance API");

    QVariant key = settings.value("API Key");
    if (key.isValid())
    {
        ui->apiKeyLineEdit->setText(key.toString());
    }

    QVariant code = settings.value("Secret Code");
    if (code.isValid())
    {
        ui->secretCodeLineEdit->setText(code.toString());
    }

    QVariant uri = settings.value("Redirect URI");
    if (uri.isValid())
    {
        ui->redirectUriLineEdit->setText(uri.toString());
    }
}
