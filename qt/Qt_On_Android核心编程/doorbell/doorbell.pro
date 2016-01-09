#-------------------------------------------------
#
# Project created by QtCreator 2014-05-11T13:44:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = doorbell
TEMPLATE = app


SOURCES += main.cpp \
    doorbell.cpp

HEADERS  += \
    doorbell.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    doorbell.qrc

android-g++ {
    LIBS += -lgnustl_shared
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

