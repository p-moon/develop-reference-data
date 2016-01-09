#-------------------------------------------------
#
# Project created by QtCreator 2014-04-26T13:37:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Q12
TEMPLATE = app


SOURCES += main.cpp\
        Q12Widget.cpp

HEADERS  += Q12Widget.h

CONFIG += mobility
MOBILITY = 

!win32{
  LIBS += -lgnustl_shared
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

