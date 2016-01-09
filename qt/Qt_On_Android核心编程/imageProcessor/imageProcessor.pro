#-------------------------------------------------
#
# Project created by QtCreator 2014-06-08T22:39:38
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imageProcessor
TEMPLATE = app

android-g++ {
    QT += androidextras
    LIBS += -lgnustl_shared
}

SOURCES += main.cpp\
        widget.cpp \
    imageProcessor.cpp \
    ../openFileWidget.cpp \
    ../qDebug2Logcat.cpp \
    ../simpleCustomEvent.cpp \
    ../worker.cpp \
    imageView.cpp

HEADERS  += widget.h \
    imageProcessor.h \
    ../openFileWidget.h \
    ../qDebug2Logcat.h \
    ../simpleCustomEvent.h \
    ../worker.h \
    imageView.h

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    android/src/an/qt/imageProcessor/ImageProcessorActivity.java \
    android/src/an/qt/imageProcessor/ImageCaptureNative.java

RESOURCES += \
    imageProcessor.qrc

