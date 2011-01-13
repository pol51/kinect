TEMPLATE = app
TARGET =
DEPENDPATH += . src
INCLUDEPATH += . src

QT += opengl

CFLAGS=-g -Wall `pkg-config --cflags libfreenect`
LIBS = `pkg-config --libs libfreenect`

# Input
SOURCES += src/main.cpp \
    src/mainWindow.cpp \
    src/kinectView.cpp \
    src/freenectDevice.cpp

HEADERS += \
    src/mainWindow.h \
    src/kinectView.h \
    src/freenectDevice.h
