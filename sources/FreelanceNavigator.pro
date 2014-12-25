#-------------------------------------------------
#
# Project created by QtCreator 2014-12-12T15:17:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = FreelanceNavigator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    elanceapiclient.cpp \
    elancesettingsdialog.cpp \
    elancedatareader.cpp \
    elancetokensdata.cpp \
    elancejobsdata.cpp \
    ielancedata.cpp \
    elancejobdata.cpp

HEADERS  += mainwindow.h \
    elanceapiclient.h \
    elancesettingsdialog.h \
    elancedatareader.h \
    elancetokensdata.h \
    ielancetokensdata.h \
    ielancedata.h \
    ielancejobsdata.h \
    elancejobsdata.h \
    ielancejobdata.h \
    elancejobdata.h

FORMS    += mainwindow.ui \
    elancesettingsdialog.ui
