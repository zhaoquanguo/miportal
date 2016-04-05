#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T17:23:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = binlight.cp
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

SOLUTION_ROOT = $$PWD/..
DESTDIR = $$SOLUTION_ROOT/out
message($$OUT_PWD)

INCLUDEPATH += $$SOLUTION_ROOT/3rdparty/upnp/include
LIBS += -L$$SOLUTION_ROOT/3rdparty/upnp/lib -lupnp

win32:CONFIG(release, debug|release) {
    LIBS += -L$$OUT_PWD/../miupnp/release/ -lmiupnp
    LIBS += -L$$OUT_PWD/../binlight/release/ -lbinlight
}
else:win32:CONFIG(debug, debug|release) {
    LIBS += -L$$OUT_PWD/../miupnp/debug/ -lmiupnp
    LIBS += -L$$OUT_PWD/../binlight/debug/ -lbinlight
}
else:unix {
    LIBS += -L$$OUT_PWD/../miupnp/ -lmiupnp
    LIBS += -L$$OUT_PWD/../binlight/ -lbinlight
}

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

RESOURCES += \
    images.qrc
