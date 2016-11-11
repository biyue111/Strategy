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

    Node(int i_x, int i_y, Node *i_parent = NULL){
		x = i_x;y = i_y;
		parent = i_parent;
        //gameMapRegion = i_gameMapRegion;
	}
    //Node *parent
};

class NPCPlayer : public Player
{
	Q_OBJECT;
private:
	QVector<QVector<GameMapRegion *>> *gMR;
public:
    NPCPlayer(int i_id, QVector<QVector<GameMapRegion *>> *i_gMR);
    QVector<Node *> searchPath(int sx, int sy, int dx, int dy);
	void excuteAI();
};

#endif // NPCPLAYER_H
