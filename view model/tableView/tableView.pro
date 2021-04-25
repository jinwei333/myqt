#-------------------------------------------------
#
# Project created by QtCreator 2021-01-27T08:57:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tableView
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    comboxdelegate.cpp \
    main.cpp \
    mainwindow.cpp \
    spinboxdelegate.cpp

HEADERS += \
    comboxdelegate.h \
    mainwindow.h \
    spinboxdelegate.h

FORMS    += mainwindow.ui
