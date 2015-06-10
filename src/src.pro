TEMPLATE = lib
QT       = core network
CONFIG  += c++11
TARGET   = qmhd

LIBS    += -lmicrohttpd

HEADERS += \
    qmhdbody.h \
    qmhdbodyfile.h \
    qmhdcontroller.h \
    qmhdglobal.h \
    qmhdrequest.h \
    qmhdresponse.h \
    qmhdroute.h \
    qmhdrouter.h \
    qmhdserver.h

SOURCES += \
    qmhdbody.cpp \
    qmhdbodyfile.cpp \
    qmhdcontroller.cpp \
    qmhdglobal.cpp \
    qmhdrequest.cpp \
    qmhdresponse.cpp \
    qmhdroute.cpp \
    qmhdrouter.cpp \
    qmhdserver.cpp
