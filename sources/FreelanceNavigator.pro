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

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

SOURCES += main.cpp\
        mainwindow.cpp \
    elanceapiclient.cpp \
    elancesettingsdialog.cpp \
    elancedatareader.cpp \
    elancetokens.cpp \
    elancejobspage.cpp \
    elancejob.cpp \
    elancecategory.cpp \
    cookiejar.cpp \
    aboutdialog.cpp \
    jobitemdelegate.cpp \
    elanceerror.cpp \
    jobsloader.cpp \
    elanceapirequest.cpp \
    tokensrequest.cpp \
    gettokensrequest.cpp \
    refreshtokensrequest.cpp \
    categoriesrequest.cpp \
    datarequest.cpp \
    jobsrequest.cpp

HEADERS  += mainwindow.h \
    elanceapiclient.h \
    elancesettingsdialog.h \
    elancedatareader.h \
    ielancedata.h \
    ielancetokens.h \
    elancetokens.h \
    ielancejobspage.h \
    elancejobspage.h \
    ielancejob.h \
    elancejob.h \
    ielancecategory.h \
    elancecategory.h \
    cookiejar.h \
    aboutdialog.h \
    jobitemdelegate.h \
    ielanceerror.h \
    elanceerror.h \
    jobsloader.h \
    elanceapirequest.h \
    tokensrequest.h \
    gettokensrequest.h \
    refreshtokensrequest.h \
    categoriesrequest.h \
    datarequest.h \
    jobsrequest.h

FORMS    += mainwindow.ui \
    elancesettingsdialog.ui \
    aboutdialog.ui

win32 {
    RC_ICONS = Resources/FreelanceNavigator.ico
}
