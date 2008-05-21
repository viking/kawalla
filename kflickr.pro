TEMPLATE = app
CONFIG += qt debug
INCLUDEPATH += . /usr/include/kde
LIBS += -L/usr/lib/kde3 -lkdeui -lkio -lm

HEADERS += window.h
HEADERS += handler.h
SOURCES += window.cpp
SOURCES += handler.cpp
SOURCES += main.cpp
