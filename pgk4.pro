TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
LIBS   += -lepoxy
LIBS   += -lGL
LIBS   += -lglfw

SOURCES += \
    Wojciech_Strak_03.cpp \
    AGL3Window.cpp \
    backgroundrectangle.cpp \
    camera.cpp \
    myshape.cpp \
    mysphere.cpp
HEADERS += \
    AGL3Window.hpp \
    AGL3Drawable.hpp \
    backgroundrectangle.h \
    camera.h \
    myshape.h \
    triangle.h \
    mysphere.h \
    myShape.vs \
    myShape.fs \
    MySphere.vs \
    MySphere.fs
