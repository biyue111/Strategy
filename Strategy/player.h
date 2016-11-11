#ifndef PLAYER_H
#define PLAYER_H
#include <QVector>
#include <QObject>
#include "army.h"
#include "gamemapregion.h"


class Player : public QObject
{
    Q_OBJECT
signals:
    void tryMoveArmy(QGraphicsItem *army, int hexCoorX, int hexCoorY);
	void endTurn();
protected:
	int id;
    bool is_NPCPlayer;
    QVector<Army *> armyVList;
	QVector<GameMapRegion *> cityVList;
public:
    Player(int i_id, bool i_NPCPlayer);
	//void excuteAI();
	void enableInteraction();
	void disableInteraction();
	void addArmy(Army *army);
	int getPlayerID() {return id;}
    bool isNPC(){return is_NPCPlayer;}
    QVector<Army *> *getArmyList(){return &armyVList;}
    QVector<GameMapRegion *> *getCityVList(){return &cityVList;}
	//AI
	//NPCPlayerAI *ai;
};

#endif // PLAYER_H
