#-------------------------------------------------
#
# Project created by QtCreator 2013-04-14T16:51:37
#
#-------------------------------------------------

QT       += core gui
QT += network

TARGET = webdebug
TEMPLATE = app


SOURCES += main.cpp\
        wdebug.cpp \
    subwin.cpp \
    monitor.cpp \
    wdserver.cpp \
    wdsocket.cpp \
    wdworker.cpp \
    wdglobal.cpp \
    wddatar.cpp

HEADERS  += wdebug.h \
    subwin.h \
    monitor.h \
    wdserver.h \
    wdsocket.h \
    wdworker.h \
    wdglobal.h \
    wddatar.h \
    wddef.h

FORMS    += wdebug.ui \
    subwin.ui \
    monitor.ui
