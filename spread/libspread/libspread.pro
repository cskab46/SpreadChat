TARGET = spread
TEMPLATE = lib

CONFIG -= qt
unix:CONFIG += static
win32:CONFIG += shared

INCLUDEPATH += ../include ../libspread-util/include ../stdutil/src ../daemon

HEADERS += \
    fl_p.h \
    scatp.h \
    ../include/fl.h \
    ../include/sp.h \
    ../include/sp_events.h \
    ../include/sp_func.h

SOURCES += \
    fl.c \
    scatp.c \
    sp.c
