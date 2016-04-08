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
    miupnpaction.h \
    Upnp.h

#The upnp include directories.
INCLUDEPATH += ../3rdparty/UpnpLan/src/3rdpart_src/expat/lib/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Base/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Container/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Http/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Log/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Memory/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Socket/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/String/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Tcp/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Thread/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Timer/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Uuid/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Worker/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Xml/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Net/
INCLUDEPATH += ../3rdparty/UpnpLan/src/Tiny/Md5/
INCLUDEPATH += ../3rdparty/UpnpLan/src/UpnpStack/UpnpInitializer/
INCLUDEPATH += ../3rdparty/UpnpLan/src/UpnpStack/UpnpCommon/
INCLUDEPATH += ../3rdparty/UpnpLan/src/UpnpStack/UpnpTypedef/
INCLUDEPATH += ../3rdparty/UpnpLan/src/UpnpStack/UpnpHttp/
INCLUDEPATH += ../3rdparty/UpnpLan/src/UpnpStack/UpnpProvider/
INCLUDEPATH += ../3rdparty/UpnpLan/src/UpnpStack/UpnpRegistry/
INCLUDEPATH += ../3rdparty/UpnpLan/src/UpnpStack/UpnpCtrlPoint/
INCLUDEPATH += ../3rdparty/UpnpLan/src/UpnpStack/UpnpHost/
INCLUDEPATH += ../3rdparty/UpnpLan/src/UpnpStack/UpnpRuntime/

OTHER_FILES = ../3rdparty/bin/upnp.dll
OTHER_FILES += ../3rdparty/bin/expat.dll

# Copy the dynamic library.
CONFIG(debug, debug|release) {
    OTHER_LIB_DIR = ../3rdparty/lib/debug/
}
CONFIG(release, debug|release) {
    OTHER_LIB_DIR = ../3rdparty/lib/release/
}

win32 {
    DESTDIR_WIN = $${DESTDIR}
    DESTDIR_WIN ~= s,/,\\,g
    SRC_DLLS_WIN = $${OTHER_LIB_DIR}*.dll
    SRC_DLLS_WIN ~= s,/,\\,g

    QMAKE_PRE_LINK += $$quote(cmd /c copy /y $${SRC_DLLS_WIN} $${DESTDIR_WIN}$$escape_expand(\\n\\t))
}





