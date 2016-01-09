#-------------------------------------------------
#
# Project created by QtCreator 2014-05-18T09:58:25
#
#-------------------------------------------------

QT       += core gui multimedia sensors

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aMusic
TEMPLATE = app

android-g++ {
    LIBS += -lgnustl_shared
}

SOURCES += main.cpp\
        widget.cpp \
    ../openFileWidget.cpp \
    ../worker.cpp \
    ../qDebug2Logcat.cpp \
    ../imageButton.cpp \
    musicWidget.cpp

HEADERS  += widget.h \
    ../openFileWidget.h \
    ../worker.h \
    ../qDebug2Logcat.h \
    ../imageButton.h \
    musicWidget.h \
    rowDelegate.h

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    amusic.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

