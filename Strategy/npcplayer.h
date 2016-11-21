#ifndef NPCPLAYER_H
#define NPCPLAYER_H
#include <QObject>
#include <QQueue>
#include "player.h"

class Node
{
public:
    Node *parent;
    int x,y;
	int cost;

    Node(int i_x, int i_y, Node *i_parent = NULL, int i_cost = 0){
		x = i_x;y = i_y;
		parent = i_parent;
		cost = i_cost;
        //gameMapRegion = i_gameMapRegion;
	}
    //Node *parent
};

class NPCPlayer : public Player
{
	Q_OBJECT;
private:
	QVector<QVector<GameMapRegion *>> *gMR;
	QVector<GameMapRegion *> wantedCityVList;
    QVector<Node *> searchShortestPath(int sx, int sy, int dx, int dy);
    QVector<Node *> searchPathA(int sx, int sy, int dx, int dy, double fightCostWight);
public:
    NPCPlayer(int i_id, QVector<QVector<GameMapRegion *>> *i_gMR);
	void excuteAI();
	void findWantedCity();
};

#endif // NPCPLAYER_H
