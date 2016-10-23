TARGET = SpreadChat
TEMPLATE = app

CONFIG += c++11
QT += core gui widgets

INCLUDEPATH += ../spread/include

unix:LIBS += \
    ../spread/libspread/libspread.a \
    ../spread/libspread-util/libspread-util.a \
    ../spread/stdutil/libstdutil.a \
    -ldl

SOURCES += \
    main.cpp
