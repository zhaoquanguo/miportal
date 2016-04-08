INCLUDEPATH += ..

CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug
    LIBS += -L../3rdparty/lib/debug

}
CONFIG(release, debug|release) {
    DESTDIR = ../build/release
    LIBS += -L../3rdparty/lib/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u
