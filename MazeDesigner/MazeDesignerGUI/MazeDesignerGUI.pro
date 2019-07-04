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
        ../Common/common.cpp \
        ../ConditionParser/condition.cpp \
        ../ConditionParser/simplecondition.cpp \
        ../ConditionParser/compositecondition.cpp \
        ../Explorer/*.cpp \
    instancecanvas.cpp \
    doorinstance.cpp \
    keyinstance.cpp \
    selectableinstance.cpp \
    roomlistwidget.cpp \
    exporter.cpp \
    console.cpp \
    doorconditionconsole.cpp

HEADERS += \
        mainwindow.h \
        ../Canvas/*.h \
        ../Keys/*.h \
        ../ConditionParser/*.h \
    instancecanvas.h \
    doorinstance.h \
    keyinstance.h \
    selectableinstance.h \
    roomlistwidget.h \
    exporter.h \
    console.h \
    doorconditionconsole.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../key.png \
    ../res/solidInstanceTemplate.json \
    ../res/roomResourceTemplate.json \
    ../res/roomTemplate.yy \
    ../res/obj_wall/obj_wall.yy \
    ../res/spr_wall/990f4d0f-c443-4406-9216-dbc6bbf5ccf6.png \
    ../res/spr_wall/spr_wall.yy \
    ../res/spr_wall/layers/990f4d0f-c443-4406-9216-dbc6bbf5ccf6/a6ae5f8f-dceb-4020-8bae-4c34b10d97ca.png
