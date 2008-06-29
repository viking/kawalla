TEMPLATE = app
CONFIG += qt debug
INCLUDEPATH += . /usr/include/kde
LIBS += -L/usr/lib/kde3 -lkdeui -lkio -lm `Magick++-config --libs`
DEFINES += PWD="\"$$(PWD)\""

HEADERS += window.h
HEADERS += handler.h
HEADERS += row.h
HEADERS += photo.h
SOURCES += window.cpp
SOURCES += handler.cpp
SOURCES += row.cpp
SOURCES += main.cpp
