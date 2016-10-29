#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T17:00:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Strategy
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    GameBackGround.cpp \
    gameinterface.cpp \
    army.cpp \
    gameutil.cpp

HEADERS  += widget.h \
    GameBackGround.h \
    GameUtil.h \
    gameinterface.h \
    army.h

FORMS    += widget.ui
