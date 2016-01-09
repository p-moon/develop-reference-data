#-------------------------------------------------
#
# Project created by QtCreator 2014-05-11T16:31:46
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imageloader
TEMPLATE = app


android-g++ {
    LIBS += -lgnustl_shared
}

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    imageloader.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

