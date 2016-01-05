#-------------------------------------------------
#
# Project created by QtCreator 2013-07-03T11:54:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
qtHaveModule(printsupport): QT += printsupport
TARGET = MyselfWord
TEMPLATE = app


SOURCES += main.cpp\
        myword.cpp \
    mychild.cpp

HEADERS  += myword.h \
    mychild.h

RESOURCES += \
    myword.qrc
