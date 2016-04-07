!include( ../miportral.pri ) {
    error( "Couldn't find the miportral.pri file!" )
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = binlight.cp
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc


LIBS += -L$$DESTDIR
LIBS += -lmiupnp -lbinlight

#
LIBS += -L../3rdparty/upnp/lib
LIBS += -lupnp

