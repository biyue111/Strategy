#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T17:00:51
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Strategy
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    army.cpp \
    gameutil.cpp \
    gamemapregion.cpp \
    gamecontroler.cpp \
    player.cpp \
    npcplayer.cpp \
    networkhelp.cpp

HEADERS  += widget.h \
    GameUtil.h \
    army.h \
    gamemapregion.h \
    gamecontroler.h \
    player.h \
    npcplayer.h \
    networkhelp.h

FORMS    += widget.ui \
    networkhelp.ui

RC_FILE += Strategyico.rc
