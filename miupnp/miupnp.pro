#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T17:44:34
#
#-------------------------------------------------

QT       -= gui

TARGET = miupnp
TEMPLATE = lib
CONFIG += staticlib

SOLUTION_ROOT = $$PWD/..

INCLUDEPATH += $$SOLUTION_ROOT/3rdparty/upnp/include
LIBS += -L$$SOLUTION_ROOT/3rdparty/upnp/lib -lupnp

SOURCES += miupnp.cpp \
    miupnpruntime.cpp \
    miupnpdevice.cpp \
    miupnpservice.cpp \
    miupnpevent.cpp \
    miupnpaction.cpp

HEADERS += miupnp.h\
    miupnpruntime.h \
    miupnpdevice.h \
    miupnpservice.h \
    miupnpevent.h \
    miupnpaction.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


