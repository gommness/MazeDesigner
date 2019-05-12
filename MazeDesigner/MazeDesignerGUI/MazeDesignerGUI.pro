#-------------------------------------------------
#
# Project created by QtCreator 2019-04-20T03:47:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MazeDesignerGUI
TEMPLATE = app

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

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        ../Canvas/canvas.cpp \
        ../Canvas/grid.cpp \
        ../Canvas/room.cpp \
        ../Canvas/roomcanvas.cpp \
        ../Canvas/shape.cpp \
        ../Keys/keylistwidget.cpp \
        ../Keys/key.cpp \
        ../Keys/keyeditorwidget.cpp \
        ../Keys/keyispowerupeditor.cpp \
        ../Keys/keynameeditor.cpp \
        ../Keys/keyrepository.cpp \
    instancecanvas.cpp \
    doorinstance.cpp \
    keyinstance.cpp

HEADERS += \
        mainwindow.h \
        ../Canvas/*.h \
        ../Keys/*.h \
    instancecanvas.h \
    doorinstance.h \
    keyinstance.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../key.png
