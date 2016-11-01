#ifndef GAMECONTROLER_H
#define GAMECONTROLER_H
#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLayout>
#include <QObject>
#include <QPushButton>
#include <QVector>
#include "GameBackGround.h"
#include "army.h"
#include "gameutil.h"
#include "gamemapregion.h"

class GameControler : public QObject
{
    Q_OBJECT
private:
    QGraphicsScene *mainGameMap;
    QGraphicsScene *activePlayer;
    QGraphicsView *view;
    QPushButton *nextTurnBtn;
    QVector<QVector<GameMapRegion *>> gameMapRegion;

    void moveArmy(Army *army, GameMapRegion *r);
    void armyFight(Army *attacker, Army *defender);
public:
    GameControler(QWidget *w);
    void createGameMap(QGraphicsScene *scene);
    void createArmy(int hexCoorX, int hexCoorY, int owner, bool hasgeneral = false);
    ~GameControler() {}
public slots:
    void tryMoveArmy(QGraphicsItem *army, int hexCoorX, int hexCoorY);
};

#endif // GAMECONTROLER_H
