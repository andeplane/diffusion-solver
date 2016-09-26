TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cell.cpp \
    grid.cpp \
    geometry.cpp \
    integrator.cpp

HEADERS += \
    cell.h \
    common.h \
    grid.h \
    geometry.h \
    integrator.h
