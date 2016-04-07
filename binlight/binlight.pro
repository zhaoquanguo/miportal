!include( ../miportral.pri ) {
    error( "Couldn't find the miportral.pri file!" )
}

QT       -= gui

TARGET = binlight
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    binlight_stub.cpp

HEADERS += \
    binlight_stub.h



