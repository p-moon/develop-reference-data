#-------------------------------------------------
#
# Project created by QtCreator 2014-05-10T23:28:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LinearLabel
TEMPLATE = app


SOURCES += main.cpp \
    linearlabel.cpp

HEADERS  += \
    linearlabel.h

android-g++ {
    LIBS += -lgnustl_shared
}
CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

