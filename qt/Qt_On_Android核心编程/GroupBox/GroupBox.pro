#-------------------------------------------------
#
# Project created by QtCreator 2014-05-08T23:35:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GroupBox
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

CONFIG += mobility
MOBILITY = 

android-g++ {
    LIBS += -lgnustl_shared
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

