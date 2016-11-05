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
#include <fstream>
#include "GameBackGround.h"
#include "army.h"
#include "gameutil.h"
#include "gamemapregion.h"
#include "player.h"

class GameControler : public QObject
{
    Q_OBJECT
private:
    QGraphicsScene *mainGameMap;
    QGraphicsScene *activePlayer;
    QGraphicsView *view;
    QPushButton *nextTurnBtn;
    QVector<QVector<GameMapRegion *>> gameMapRegion;
    QVector<GameMapRegion *> cityVList;
	QVector<Army *> armyVList;
	QVector<int> playerIDVList;
	QMap<int, Player *> idPlayerMap;
	int actPlayerSequenceNumber;
    Player *actPlayer;

    void moveArmy(Army *army, GameMapRegion *r);
    void armyFight(Army *attacker, GameMapRegion *r);
    void armyRegoup(Army *army1, GameMapRegion *r);
public:
    GameControler(QWidget *w);
    void createGameMap(QGraphicsScene *scene);
	//Game Process Control
	void gameBegin();
	void onePlayerFinish();
    void endTurn();

    void createPlayer(int id, bool i_isNPC);
	void createArmy(int hexCoorX, int hexCoorY, int owner, 
                     int number = 1, bool hasgeneral = false);
	void removeArmy(Army *army);
	void activatePlayer(Player *player);
    void inactivatePlayer();
    ~GameControler() {}
public slots:
    void tryMoveArmy(QGraphicsItem *army, int hexCoorX, int hexCoorY);
    void armyClicked(QGraphicsItem *i_army, bool state);
    void nextTurnBtnPushed();
};

#endif // GAMECONTROLER_H
