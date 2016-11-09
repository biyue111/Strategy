#ifndef PLAYER_H
#define PLAYER_H
#include <QVector>
#include "army.h"
#include "gamemapregion.h"


class Player
{
private:
	int id;
	bool NPCPlayer;
    QVector<Army *> armyVList;
	QVector<GameMapRegion *> cityVList;
	//AI
public:
    Player(int i_id, bool i_NPCPlayer);
	void excuteAI();
	void enableInteraction();
	void disableInteraction();
	void addArmy(Army *army);
	int getPlayerID() {return id;}
    bool isNPC(){return NPCPlayer;}
    QVector<Army *> *getArmyList(){return &armyVList;}
    QVector<GameMapRegion *> *getCityVList(){return &cityVList;}
};

#endif // PLAYER_H
