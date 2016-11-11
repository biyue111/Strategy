#include "npcplayer.h"

NPCPlayer::NPCPlayer(int i_id, QVector<QVector<GameMapRegion *>> * i_gMR) : Player(i_id, true)
{
    gMR = i_gMR;
    id = i_id;
    is_NPCPlayer = true;
}

void NPCPlayer::excuteAI(){
	int i = 0;
    while(i < armyVList.size()){
        if(!armyVList[i]->moved){
            Army *army = armyVList[i];
            QVector <Node *> path = searchPath(army->getHexCoorX(), army->getHexCoorY(), 1 ,1);
            if(path.size() >= 2){
                Node *firstNode = path[path.size()-2];
                qDebug()<<"moveArmyto"<< firstNode->x <<" "<<firstNode->y;
                int nx,ny;
                for(int k=0;k<18;k++){
                    intCoor coor = GameUtil::getDesMapHexCoor(army->getHexCoorX(), army->getHexCoorY(),
                                                              GameUtil::nearbyHexCoor[k][0], GameUtil::nearbyHexCoor[k][1]);
                    if(coor.first == firstNode->x && coor.second == firstNode->y){
                        nx = GameUtil::nearbyHexCoor[k][0];
                        ny = GameUtil::nearbyHexCoor[k][1];
                        break;
                    }
                }
                emit(tryMoveArmy(army, nx, ny));
                //army->moved = true;
            }
        }
        i++;
    }
    emit(endTurn());
}

QVector<Node *> NPCPlayer::searchPath(int sx, int sy, int dx, int dy){
	QQueue<Node *> qq;
	QVector<Node *> res;
    bool visited[GAMEBG_GRID_COLONM][GAMEBG_GRID_ROW];
	bool findFlag;
	QVector<QVector<GameMapRegion *>> gameMap = *gMR;
    for(int i=0;i<GAMEBG_GRID_COLONM;i++)
        for(int j=0;j<GAMEBG_GRID_ROW;j++)
            visited[i][j] = false;
	qq.enqueue(new Node(sx,sy));
    visited[sx][sy] = true;
	while(qq.size() > 0 && !findFlag){
		Node *c = qq.dequeue();
        //qDebug() <<"dequeue: "<< c->x <<" "<<c->y;
		if(c->x == dx && c->y == dy){
			findFlag = true;
			Node *node = c;
			res.append(node);
			while(node->parent != NULL){
				qDebug() << node->x <<" "<< node->y;
				node = node->parent;
				res.append(node);
			}
		} else{
			for(int i=0;i<18;i++){
				bool inerObstruct[6];
                intCoor newCoor = GameUtil::getDesMapHexCoor(c->x, c->y,
                                        GameUtil::nearbyHexCoor[i][0], GameUtil::nearbyHexCoor[i][1]);
                qDebug() <<"test: "<< newCoor.first <<" "<<newCoor.second;
                bool accessible = false;
				if(!GameUtil::inMap(newCoor.first, newCoor.second))
					accessible = false;
				else{
                    GameMapRegion *srcRegion = gameMap[c->x][c->y];
					GameMapRegion *desRegion = gameMap[newCoor.first][newCoor.second];
					//Analyse landform
					bool sameLandForm;
					if(srcRegion->getLandForm() == desRegion->getLandForm())
						sameLandForm = true;
					else if(srcRegion->getLandForm() != GameUtil::ocean &&
							desRegion->getLandForm() != GameUtil::ocean)
						sameLandForm = true;
					else 
						sameLandForm = false;

					if(i < 6){// iner circle rang = 1
						accessible = true;
						if(!sameLandForm || desRegion->getArmy() != Q_NULLPTR ||
								desRegion->getLandForm() == GameUtil::city)
							inerObstruct[i] = true;
						else 
							inerObstruct[i] = false;
					} else{// i >= 6
						//iner Obstruct ?
						bool obstructecd;
						if( i % 2 == 0){
							obstructecd = inerObstruct[(i - 6) / 2];
						} else{
							obstructecd = inerObstruct[(i - 6) / 2] && inerObstruct[((i - 6) / 2 + 1) % 6];
						}

						if(!sameLandForm || obstructecd)
							accessible = false;
						else
							accessible = true;
					}
				}
                if(accessible && !visited[newCoor.first][newCoor.second]){
                    //qDebug() <<"enqueue: "<< newCoor.first <<" "<<newCoor.second;
                    qq.enqueue(new Node(newCoor.first, newCoor.second, c));
                    visited[newCoor.first][newCoor.second] = true;
				}
			}
		}
	}
    return res;
}
