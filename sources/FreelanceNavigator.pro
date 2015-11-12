#-------------------------------------------------
#
# Project created by QtCreator 2014-12-12T15:17:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = FreelanceNavigator
TEMPLATE = app

VERSION = 1.0.0.0

# Enable C++14
CONFIG += c++14

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

include(Widgets/Widgets.pri)
include(QtSingleApplication/QtSingleApplication.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

win32 {
    RC_ICONS = Resources/FreelanceNavigator.ico
}

RESOURCES += \
    freelancenavigator.qrc
