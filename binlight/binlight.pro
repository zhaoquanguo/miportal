#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T20:45:21
#
#-------------------------------------------------

QT       -= gui

TARGET = binlight
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    binlight_stub.cpp

HEADERS += \
    binlight_stub.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../miupnp/release/ -lmiupnp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../miupnp/debug/ -lmiupnp

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../miupnp/release/libmiupnp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../miupnp/debug/libmiupnp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../miupnp/release/miupnp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../miupnp/debug/miupnp.lib
