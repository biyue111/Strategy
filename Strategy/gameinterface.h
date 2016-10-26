#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLayout>
#include <QObject>
#include <QPushButton>
#include "GameBackGround.h"
#include "GameUtil.h"

class GameInterface
{
public:
    GameInterface(QWidget *);

    //QWidget *mainWidget;
    QGraphicsScene *mainGameMap;
    QGraphicsScene *activePlayer;
    QGraphicsView *view;
    QPushButton *nextTurnBtn;
};

#endif // GAMEINTERFACE_H
