#-------------------------------------------------
#
# Project created by QtCreator 2014-06-10T08:45:24
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qWeather
TEMPLATE = app

android-g++ {
    QT += androidextras
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
    android/src/an/qt/qWeather/QWeatherActivity.java \
    android/src/an/qt/qWeather/QWeatherNative.java

