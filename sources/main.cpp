#include "elanceapiclient.h"
#include "elancesettingsdialog.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // Setting the Application version
    application.setApplicationVersion(APP_VERSION);

    QCoreApplication::setOrganizationName("ElanGroup");
    QCoreApplication::setApplicationName("Freelance Navigator");

    FreelanceNavigator::ElanceApiClient elanceApiClient;
    while (!elanceApiClient.readSettings())
    {
        FreelanceNavigator::ElanceSettingsDialog settingsDialog;
        int result = settingsDialog.exec();
        if (result == QDialog::Rejected)
        {
            return 0;
        }
    }

    if (elanceApiClient.authorize())
    {
        FreelanceNavigator::MainWindow mainWindow(&elanceApiClient);
        mainWindow.show();
        return application.exec();
    }

    return 0;
}
