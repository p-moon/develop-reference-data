#-------------------------------------------------
#
# Project created by QtCreator 2013-06-23T09:20:53
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SQLEx
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connectdlg.cpp \
    editdialog.cpp

HEADERS  += mainwindow.h \
    connectdlg.h \
    editdialog.h

FORMS += \
    connectdlg.ui
