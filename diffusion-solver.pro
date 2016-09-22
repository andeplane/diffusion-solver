TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/local/include

SOURCES += main.cpp \
    cell.cpp \
    grid.cpp \
    geometry.cpp

HEADERS += \
    cell.h \
    common.h \
    grid.h \
    geometry.h
