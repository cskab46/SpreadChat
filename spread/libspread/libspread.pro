TARGET = spread
TEMPLATE = lib

CONFIG -= qt
CONFIG += static

DEFINES += _REENTRANT

win32:DEFINES += WIN32 ARCH_PC_WIN95

INCLUDEPATH += ../include ../libspread-util/include ../stdutil/src ../daemon

HEADERS += \
    fl_p.h \
    scatp.h \
    ../include/fl.h \
    ../include/sp.h \
    ../include/sp_events.h \
    ../include/sp_func.h \
    ../daemon/status.h \
    ../daemon/spread_params.h \
    ../daemon/session.h \
    ../daemon/sess_types.h \
    ../daemon/sess_body.h \
    ../daemon/scatter.h \
    ../daemon/protocol.h \
    ../daemon/prot_objs.h \
    ../daemon/prot_body.h \
    ../daemon/network.h \
    ../daemon/net_types.h \
    ../daemon/mutex.h \
    ../daemon/message.h \
    ../daemon/membership.h \
    ../daemon/log.h \
    ../daemon/ip_enum.h \
    ../daemon/groups.h \
    ../daemon/flow_control.h \
    ../daemon/errors.h \
    ../daemon/defines.h \
    ../daemon/configuration.h \
    ../daemon/conf_body.h \
    ../daemon/auth-pword-client.h \
    ../daemon/arch.h \
    ../daemon/acm.h \
    ../daemon/acconfig.h

SOURCES += \
    fl.c \
    scatp.c \
    sp.c
