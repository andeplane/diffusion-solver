TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -g
#DEFINES += DEBUG
SOURCES += main.cpp \
    cell.cpp \
    grid.cpp \
    geometry.cpp \
    integrators/integrator.cpp \
    integrators/forwardeuler.cpp \
    modifiers/modifier.cpp \
    system.cpp \
    modifiers/fixedboundaryvalue.cpp

HEADERS += \
    cell.h \
    common.h \
    grid.h \
    geometry.h \
    integrators/integrator.h \
    integrators/forwardeuler.h \
    modifiers/modifier.h \
    modifiers/modifiers.h \
    system.h \
    modifiers/fixedboundaryvalue.h
