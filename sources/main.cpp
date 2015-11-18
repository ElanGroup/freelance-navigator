#include "mainwindow.h"
#include <QtSingleApplication/qtsingleapplication.h>

int main(int argc, char *argv[])
{
    QtSingleApplication application(argc, argv);

    // Check if another instance of application is running.
    if (application.isRunning())
    {
        return 0;
    }

    // Setting the Application version
    application.setApplicationVersion(APP_VERSION);

    QCoreApplication::setOrganizationName("ElanGroup");
    QCoreApplication::setApplicationName("Freelance Navigator");

    FreelanceNavigator::MainWindow mainWindow;
    mainWindow.show();
    return application.exec();
}
