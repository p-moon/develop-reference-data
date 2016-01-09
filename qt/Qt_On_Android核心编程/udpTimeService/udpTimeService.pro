#-------------------------------------------------
#
# Project created by QtCreator 2014-05-17T10:51:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = udpTimeService
TEMPLATE = app

win32 {
    LIBS += -lws2_32
}

android-g++{
    LIBS += -lgnustl_shared
}

SOURCES += main.cpp\
        widget.cpp \
    timeClient.cpp \
    timeService.cpp

HEADERS  += widget.h \
    timeClient.h \
    timeService.h

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

