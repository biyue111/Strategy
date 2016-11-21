#ifndef GAMECONTROLER_H
#define GAMECONTROLER_H
#include <QObject>
#include <QWidget>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QVector>
#include <QMessageBox>
#include <fstream>
#include <QFontDatabase>
//#include "GameBackGround.h"
#include "networkhelp.h"
#include "army.h"
#include "gameutil.h"
#include "gamemapregion.h"
#include "player.h"
#include "NPCPlayer.h"

#define Player_NUMBER 4
#define HUMAN_NUMBER 2
#define RELEASE 1
//#define WINDOW_HIGHT
//#define NPC_NUMBER 0


class GameControler : public QObject
{
    Q_OBJECT
private:
    QGraphicsScene *mainGameMap;
    //QGraphicsScene *activePlayer;
    QGraphicsView *view;
    QLabel *mainLabel;
    QPushButton *helpBtn;
    QPushButton *nextTurnBtn;
    QPushButton *restartBtn;
    QVector<QVector<GameMapRegion *>> gameMapRegion;
    QVector<GameMapRegion *> cityVList;
	QVector<Army *> armyVList;
	QVector<int> playerIDVList;
	QMap<int, Player *> idPlayerMap;
	int actPlayerSequenceNumber;
    Player *actPlayer;
    bool gameEnd;
    std::default_random_engine generator;
    int turnNumber;

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
	bool playerFail(Player *player);
	int tryEndGame();

    Player *createPlayer(bool i_isNPC, int id = -1);
	void removePlayer(Player *player);
	void createArmy(int hexCoorX, int hexCoorY, int owner, 
                     int number = 1, bool hasgeneral = false);
	//void removeArmy(int id);
	void removeArmy(Army *army);
	void activatePlayer(Player *player);
    void inactivatePlayer();
    ~GameControler() {}
public slots:
    void tryMoveArmy(QGraphicsItem *army, int hexCoorX, int hexCoorY);
    void armyClicked(QGraphicsItem *i_army, bool state);
    void aiNextTurn();
    void helpBtnPushed();
    void nextTurnBtnPushed();
	void restartGame();
};

#endif // GAMECONTROLER_H
