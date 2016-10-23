TARGET = stdutil
TEMPLATE = lib

CONFIG -= qt
CONFIG += static

INCLUDEPATH += ./src

SOURCES += \
    src/stdarr.c \
    src/stdcarr.c \
    src/stddll.c \
    src/stderror.c \
    src/stdfd.c \
    src/stdhash.c \
    src/stdit.c \
    src/stdskl.c \
    src/stdthread.c \
    src/stdtime.c \
    src/stdutil.c

HEADERS += \
    src/stdutil/stdarr.h \
    src/stdutil/stdcarr.h \
    src/stdutil/stddefines.h \
    src/stdutil/stddll.h \
    src/stdutil/stderror.h \
    src/stdutil/stdfd.h \
    src/stdutil/stdhash.h \
    src/stdutil/stdit.h \
    src/stdutil/stdskl.h \
    src/stdutil/stdthread.h \
    src/stdutil/stdtime.h \
    src/stdutil/stdutil.h \
    src/stdutil/private/stdarch.h \
    src/stdutil/private/stdarch_wintel32.h \
    src/stdutil/private/stdarr_p.h \
    src/stdutil/private/stdcarr_p.h \
    src/stdutil/private/stddll_p.h \
    src/stdutil/private/stdhash_p.h \
    src/stdutil/private/stdit_p.h \
    src/stdutil/private/stdskl_p.h \
    src/stdutil/private/stdthread_p.h
