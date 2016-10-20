TARGET = core
TEMPLATE = lib

CONFIG += console c++14
CONFIG -= qt
QMAKE_CXXFLAGS += -g -fopenmp
QMAKE_LFLAGS += -fopenmp

DEFINES += DEBUG
SOURCES += grid.cpp \
    geometry.cpp \
    integrators/integrator.cpp \
    integrators/forwardeuler.cpp \
    modifiers/modifier.cpp \
    system.cpp \
    modifiers/fixedboundaryvalue.cpp \
    common.cpp \
    vec3.cpp \
    library.cpp

HEADERS += \
    common.h \
    grid.h \
    geometry.h \
    integrators/integrator.h \
    integrators/forwardeuler.h \
    modifiers/modifier.h \
    modifiers/modifiers.h \
    system.h \
    modifiers/fixedboundaryvalue.h \
    vec3.h \
    library.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
