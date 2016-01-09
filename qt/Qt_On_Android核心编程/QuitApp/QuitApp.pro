#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T14:06:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuitApp
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

CONFIG += mobility
MOBILITY = 
!win32{
  LIBS += -lgnustl_shared
}
