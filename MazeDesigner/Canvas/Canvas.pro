# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = Canvas


#-------------------------------------------------
#
# Project created by QtCreator 2019-04-04T18:00:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Canvas
TEMPLATE = lib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

HEADERS = \
   $$PWD/canvas.h \
   $$PWD/grid.h \
   $$PWD/mainwindow.h \
   $$PWD/room.h \
   $$PWD/roomcanvas.h \
   $$PWD/shape.h \
   $$PWD/ui_mainwindow.h \
    instancecanvas.h

SOURCES = \
   $$PWD/canvas.cpp \
   $$PWD/grid.cpp \
   $$PWD/main.cpp \
   $$PWD/mainwindow.cpp \
   $$PWD/room.cpp \
   $$PWD/roomcanvas.cpp \
   $$PWD/shape.cpp \
    instancecanvas.cpp

INCLUDEPATH = \
    $$PWD/.

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DEFINES = 

