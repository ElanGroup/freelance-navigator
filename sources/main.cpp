#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // Setting the Application version
    application.setApplicationVersion(APP_VERSION);

    QCoreApplication::setOrganizationName("ElanGroup");
    QCoreApplication::setApplicationName("Freelance Navigator");

    FreelanceNavigator::MainWindow mainWindow;
    mainWindow.show();
    return application.exec();

    return 0;
}
