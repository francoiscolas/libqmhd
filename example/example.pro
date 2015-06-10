TEMPLATE = app
QT       = core network
CONFIG  += c++11
TARGET   = example

INCLUDEPATH += $$PWD/../src
LIBS        += -L$$OUT_PWD/../src -lqmhd

SOURCES += example.cpp
HEADERS += example.h
