#-------------------------------------------------
#
# Project created by QtCreator 2013-04-14T16:51:37
#
#-------------------------------------------------

QT       += core gui
QT += network
DESTDIR = ./Debug
TARGET = webdebug
TEMPLATE = app

DEFINES += QT_LARGEFILE_SUPPORT QT_SQL_LIB CORE_LIBRARY QTCREATOR_UTILS_LIB
INCLUDEPATH += ./GeneratedFiles \
    ./GeneratedFiles/Debug \
    .
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

HEADERS  += wdebug.h \
    subwin.h \
    monitor.h \
    wdserver.h \
    wdsocket.h \
    wdworker.h \
    wdglobal.h \
    wddatar.h \
    wddef.h \
    wdevents.h \
    fancytabwidget.h \
    utils/utils_global.h \
    utils/stylehelper.h \
    utils/styledbar.h \
    utils/styleanimator.h \
    utils/minisplitter.h \
    utils/manhattanstyle.h \
    utils/fancymainwindow.h \
    utils/core_global.h \
    composer.h

SOURCES += main.cpp\
        wdebug.cpp \
    subwin.cpp \
    monitor.cpp \
    wdserver.cpp \
    wdsocket.cpp \
    wdworker.cpp \
    wdglobal.cpp \
    wddatar.cpp \
    wdevents.cpp \
    fancytabwidget.cpp \
    utils/stylehelper.cpp \
    utils/styledbar.cpp \
    utils/styleanimator.cpp \
    utils/minisplitter.cpp \
    utils/manhattanstyle.cpp \
    utils/fancymainwindow.cpp \
    composer.cpp

FORMS    += wdebug.ui \
    subwin.ui \
    monitor.ui \
    composer.ui

