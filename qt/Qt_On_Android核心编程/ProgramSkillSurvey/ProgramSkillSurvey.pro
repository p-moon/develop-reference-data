#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T12:25:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProgramSkillSurvey
TEMPLATE = app


SOURCES += main.cpp\
        tablewidget.cpp

HEADERS  += tablewidget.h

CONFIG += mobility
MOBILITY = 

android-g++ {
    LIBS += -lgnustl_shared
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

