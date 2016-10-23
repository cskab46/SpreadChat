TARGET = spread-util
TEMPLATE = lib

CONFIG -= qt
CONFIG += static

win32:DEFINES += ARCH_PC_WIN95 WIN32 DISABLE_FUNCTION_NAME_LOOKUP
win32:LIBS += -lws2_32

INCLUDEPATH += ./include

HEADERS += \
    src/arch.h \
    src/defines.h \
    include/spu_alarm.h \
    include/spu_alarm_types.h \
    include/spu_compatibility.h \
    include/spu_data_link.h \
    include/spu_events.h \
    include/spu_memory.h \
    include/spu_objects.h \
    include/spu_objects_local.h \
    include/spu_scatter.h \
    include/spu_system.h

SOURCES += \
    src/alarm.c \
    src/arch.c \
    src/data_link.c \
    src/events.c \
    src/memory.c
