!include( ../miportral.pri ) {
    error( "Couldn't find the miportral.pri file!" )
}

QT       -= gui

TARGET = miupnp
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
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

INCLUDEPATH += ../3rdparty/upnp/include

OTHER_FILES = ../3rdparty/upnp/bin/upnp.dll
OTHER_FILES += ../3rdparty/upnp/bin/expat.dll

# Copy the dynamic library.
win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    for(FILE, OTHER_FILES){
        message($$FILE)
        FILE ~= s,/,\\,g
        QMAKE_PRE_LINK += $$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\\n\\t))
    }
}





