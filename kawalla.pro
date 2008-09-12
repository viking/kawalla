TEMPLATE = app
CONFIG += qt debug
INCLUDEPATH += . /usr/lib/kde4/include
LIBS += -L/usr/lib/kde4/lib -lkdeui -lkio -lm `Magick++-config --libs`
DEFINES += PWD="'\"$$(PWD)\"'"

HEADERS += window.h
HEADERS += handler.h
HEADERS += row.h
HEADERS += photo.h
HEADERS += row_box.h
SOURCES += window.cpp
SOURCES += handler.cpp
SOURCES += row.cpp
SOURCES += row_box.cpp
SOURCES += main.cpp
#The following line was inserted by qt3to4
QT += xml qt3support
