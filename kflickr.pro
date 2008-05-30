TEMPLATE = app
CONFIG += qt debug
INCLUDEPATH += . /usr/include/kde
LIBS += -L/usr/lib/kde3 -lkdeui -lkio -lm `Magick++-config --libs`

HEADERS += window.h
HEADERS += handler.h
HEADERS += dbox.h
SOURCES += window.cpp
SOURCES += handler.cpp
SOURCES += dbox.cpp
SOURCES += main.cpp
