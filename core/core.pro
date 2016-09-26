TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cell.cpp \
    grid.cpp \
    geometry.cpp \
    integrators/integrator.cpp \
    integrators/forwardeuler.cpp

HEADERS += \
    cell.h \
    common.h \
    grid.h \
    geometry.h \
    integrators/integrator.h \
    integrators/forwardeuler.h
