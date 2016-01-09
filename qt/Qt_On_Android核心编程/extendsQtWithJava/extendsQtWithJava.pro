#-------------------------------------------------
#
# Project created by QtCreator 2014-05-22T10:41:59
#
#-------------------------------------------------

QT       += core gui androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = extendsQtWithJava
TEMPLATE = app

android-g++ {
    LIBS += -lgnustl_shared
}

SOURCES += main.cpp\
        widget.cpp \
    ../qDebug2Logcat.cpp \
    ../simpleCustomEvent.cpp

HEADERS  += widget.h \
    ../qDebug2Logcat.h \
    ../simpleCustomEvent.h

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/an/qt/extendsQtWithJava/ExtendsQtWithJava.java \
    android/src/an/qt/extendsQtWithJava/ExtendsQtNative.java

