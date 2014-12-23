#ifndef ELANCESETTINGSDIALOG_H
#define ELANCESETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class ElanceSettingsDialog;
}

namespace FreelanceNavigator
{
class ElanceSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ElanceSettingsDialog(QWidget *parent = 0);
    ~ElanceSettingsDialog();

private slots:
    void validateSettings();
    void saveSettings();

private:
    void setupConnections();
    void loadSettings();

    Ui::ElanceSettingsDialog *ui;
};
}

#endif // ELANCESETTINGSDIALOG_H
