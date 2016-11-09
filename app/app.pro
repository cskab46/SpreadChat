TARGET = SpreadChat
TEMPLATE = app

CONFIG += c++14
QT += core gui widgets

INCLUDEPATH += ../spread/include ./spread

unix:LIBS += \
    ../spread/libspread/libspread.a \
    ../spread/libspread-util/libspread-util.a \
    ../spread/stdutil/libstdutil.a \
    -ldl

win32:CONFIG(debug):LIBS += \
    -L$$OUT_PWD/../spread/libspread/debug -lspread \
    -L$$OUT_PWD/../spread/libspread-util/debug -lspread-util \
    -L$$OUT_PWD/../spread/stdutil/debug -lstdutil

win32:CONFIG(release):LIBS += \
    -L$$OUT_PWD/../spread/libspread/release -lspread \
    -L$$OUT_PWD/../spread/libspread-util/release -lspread-util \
    -L$$OUT_PWD/../spread/stdutil/release -lstdutil

win32:LIBS += -lws2_32

SOURCES += \
    main.cpp \
    ChatWindow.cpp \
    spread/SpreadConnection.cpp \
    ChatTabWidget.cpp \
    ConnectionDialog.cpp \
    JoinDialog.cpp \
    spread/SpreadWorker.cpp \
    spread/SpreadMessage.cpp \
    spread/WorkerGetters.cpp

FORMS += \
    ChatWindow.ui \
    ChatTabWidget.ui \
    ConnectionDialog.ui \
    JoinDialog.ui

HEADERS += \
    ChatWindow.h \
    spread/SpreadConnection.h \
    ChatTabWidget.h \
    ConnectionDialog.h \
    JoinDialog.h \
    spread/SpreadWorker.h \
    spread/SpreadMessage.h \
    spread/WorkerGetters.h
