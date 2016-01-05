#-------------------------------------------------
#
# Project created by QtCreator 2013-06-13T14:32:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LangSwitch
TEMPLATE = app


SOURCES += main.cpp\
        langswitch.cpp

HEADERS  += langswitch.h
TRANSLATIONS = lang_en.ts \
               lang_zh.ts \
               lang_la.ts
