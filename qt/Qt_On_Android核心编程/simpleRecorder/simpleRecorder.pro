#-------------------------------------------------
#
# Project created by QtCreator 2014-06-15T11:23:27
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simpleRecorder
TEMPLATE = app

android-g++ {
    LIBS += -lgnustl_shared
}

SOURCES += main.cpp\
    chatWidget.cpp \
    ../qDebug2Logcat.cpp

HEADERS  += \
    chatWidget.h \
    ../qDebug2Logcat.h

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

