QT          += core network sql
QT          -= gui
CONFIG      += console
osx:CONFIG  -= app_bundle

TARGET       = teamyarserver
TEMPLATE     = app

PRJDIR       = ../..
include($$PRJDIR/commondir.pri)

HEADERS     += \
    clienthandler.h \
    databaseinterface.h

SOURCES     += main.cpp \
    clienthandler.cpp \
    databaseinterface.cpp

LIBS        += -lqhttp

