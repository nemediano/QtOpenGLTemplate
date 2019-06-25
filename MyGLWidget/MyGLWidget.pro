#-------------------------------------------------
#
# Project created by QtCreator 2018-05-30T17:23:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyGLWidget
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


SOURCES += \
    main.cpp \
    baseoglwidget.cpp \
    testoglwidget.cpp \
    trackball.cpp \
    mainwindow.cpp

HEADERS += \
    baseoglwidget.h \
    testoglwidget.h \
    trackball.h \
    mainwindow.h

INCLUDEPATH += \
    $$PWD/../glm \
    $$PWD/../assimp-v.5.0.0.rc1/include

LIBS += \
    -L$$PWD/../assimp-v.5.0.0.rc1/lib/ -lassimp

DISTFILES += \
    shaders/simplefrag.frag \
    shaders/simplevert.vert

RESOURCES += \
    imageresources.qrc
