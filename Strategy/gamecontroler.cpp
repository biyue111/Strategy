#include "gamecontroler.h"

GameControler::GameControler(QWidget *w){
    //mainWidget = w;
    mainGameMap = new QGraphicsScene();
    QLayout * layout = new QVBoxLayout();

    nextTurnBtn = new QPushButton("&Next Turn");
    connect(nextTurnBtn,SIGNAL(clicked()),
			this, SLOT(nextTurnBtnPushed()));	
	restartBtn = new QPushButton("Restart");
    connect(restartBtn,SIGNAL(clicked()),
			this, SLOT(restartGame()));	
    double sceneX = (0.5 + 1.5 * GAMEBG_GRID_COLONM) * HEXGON_SIDE_LENGTH;
    double sceneY = GAMEBG_GRID_ROW * ROOT_3 * HEXGON_SIDE_LENGTH;
    mainGameMap->addRect(-0.5 * HEXGON_SIDE_LENGTH,0,sceneX,sceneY);
    mainGameMap->setSceneRect(-0.5 * HEXGON_SIDE_LENGTH,0,sceneX,sceneY);
    this->createGameMap(mainGameMap);

    //army1->getArmyFigure()->setPos(20,20);

    //mainGameMap->addItem(army1->getArmyhex());

    view = new QGraphicsView(mainGameMap);
    view->setWindowTitle("Graphic View");
    view->resize(sceneX + SCENE_EGDE,sceneY + SCENE_EGDE);
    //view.show();

	layout->addWidget(restartBtn);
    layout->addWidget(nextTurnBtn);
    layout->addWidget(view);

    w->setLayout(layout);
    w->sizeHint();
}

void GameControler::createGameMap(QGraphicsScene *scene){
    /*
    for(int j=0;j<GAMEBG_GRID_COLONM;j++)
        for(int i=0;i<(GAMEBG_GRID_ROW - j % 2);i++){
            QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem( QPolygonF( QVector<QPointF>() <<
                                                                                     QPointF( 0, 0 ) <<
                                                                                     QPointF( HEXGON_SIDE_LENGTH, 0 ) <<
                                                                                     QPointF( 1.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                                                                     QPointF( HEXGON_SIDE_LENGTH, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                                                                     QPointF( 0, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                                                                     QPointF(-0.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH)), 0);
            polygonItem->setPen( QPen(Qt::black) );
            polygonItem->setBrush(Qt::NoBrush);

            double Xoffset = j * 1.5 * HEXGON_SIDE_LENGTH + 0.5 * HEXGON_SIDE_LENGTH;
            double Yoffset = (i + (double)(j % 2) / 2) * ROOT_3 * HEXGON_SIDE_LENGTH;
            polygonItem->setPos(Xoffset, Yoffset);
            scene->addItem(polygonItem);
        }
        */
    std::fstream mapfile("../Strategy/asset/map/1.map");
	if(!mapfile.is_open()){
		qDebug() << "ERROR in open mapfile";
	}
	GameUtil::landform l;
    for(int j=0;j<GAMEBG_GRID_COLONM;j++){
        QVector<GameMapRegion *> rv;
        for(int i=0;i<(GAMEBG_GRID_ROW - j % 2);i++){
			if (mapfile.eof()){
				l = GameUtil::land;
			} else{
                int in;
                mapfile >> in;
				//i = 0;
                qDebug() << "createMape:"<<in;
                l = GameUtil::landform(in);
			}
            GameMapRegion *r = new GameMapRegion(j,i,l);
            r->setZValue(1);
            rv.append(r);
			if(l == GameUtil::city)
				cityVList.append(r);

            QGraphicsPolygonItem *regionBackGround = new QGraphicsPolygonItem( QPolygonF( QVector<QPointF>() <<
                                                                                     QPointF( 0, 0 ) <<
                                                                                     QPointF( HEXGON_SIDE_LENGTH, 0 ) <<
                                                                                     QPointF( 1.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                                                                     QPointF( HEXGON_SIDE_LENGTH, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                                                                     QPointF( 0, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                                                                     QPointF(-0.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH)), 0);
            regionBackGround->setBrush(GameUtil::getQColorbyLandForm(r->getLandForm()));
            dCoor pos = GameUtil::hexgonOriginPostion(j, i);
            regionBackGround->setPos(pos.first, pos.second);
			scene->addItem(regionBackGround);

            scene->addItem(r);
        }
        gameMapRegion.append(rv);
    }
	mapfile.close();
    return ;
}

void GameControler::gameBegin(){
    //create Human Players
	//for(int i=0;i<HUMAN_NUMBER;i++){
	//	this->createPlayer(false, i + 1);
	//}
    gameEnd = false;
    this->createPlayer(false, 1);
    this->createPlayer(false, 2);
	cityVList[0]->changeOwner(playerIDVList[0]);
    createArmy(cityVList[0]->getHexCoorX(), cityVList[0]->getHexCoorY(),
                    playerIDVList[0], 8);
	if(HUMAN_NUMBER == 2){
    cityVList[cityVList.size() - 1]->changeOwner(playerIDVList[1]);
    //createArmy(cityVList[cityVList.size() - 1]->getHexCoorX(),
    //            cityVList[cityVList.size() - 1]->getHexCoorY(),
	//			playerIDVList[1], 8);
	}
    //this->createArmy(1,1,1);
    //this->createArmy(2,1,1);
    ///this->createArmy(1,2,2);
	this->activatePlayer(idPlayerMap[playerIDVList[0]]);
	actPlayerSequenceNumber = 0;
	nextTurnBtn->setEnabled(true);
	//endTurn();
}
void GameControler::onePlayerFinish(){
	inactivatePlayer();
	//find next player
    if(!gameEnd){
        actPlayerSequenceNumber++;
        if(actPlayerSequenceNumber >= playerIDVList.size()){
            this->endTurn();
            actPlayerSequenceNumber = 0;
        }
        Player *nextPlayer = idPlayerMap[playerIDVList[actPlayerSequenceNumber]];
        //Add Army for Next Player
        for(int i=0;i<cityVList.size();i++){
            if(cityVList[i]->getOwnerID() == nextPlayer->getPlayerID()){
                if(cityVList[i]->getArmy()){
                    Army *army = cityVList[i]->getArmy();
                    if(army->getArmyNumber() + 8 < MAX_ARMY_NUM){
                        army->changeArmyNumber(army->getArmyNumber() + 8);
                    } else{
                        army->changeArmyNumber(MAX_ARMY_NUM);
                    }
                } else{//not have army
                    createArmy(cityVList[i]->getHexCoorX(), cityVList[i]->getHexCoorY(),
                                cityVList[i]->getOwnerID(), 8, false);
                }
            }
        }
        activatePlayer(nextPlayer);
    }
}
void GameControler::endTurn(){
	//add army in every city
	//int armdyI = 8;
}
bool GameControler::playerFail(Player *player){
	if(player)
	return !player->getArmyList()->size() && 
			!player->getCityVList()->size();
	else
		return false;
}

int GameControler::tryEndGame(){
	if(idPlayerMap.size() == 1){
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.exec();
        gameEnd = true;
		nextTurnBtn->setEnabled(false);
        return 1;
	} else
		return 0;
}

void GameControler::restartGame(){
	while(armyVList.size()){
		removeArmy(armyVList[0]);
	}
	while(idPlayerMap.size()){
        Player *player = idPlayerMap[playerIDVList[0]];
		removePlayer(player);
	}
//	mainGameMap->update();
	armyVList.clear();
	idPlayerMap.clear();
	playerIDVList.clear();
	actPlayer = Q_NULLPTR;
    for(int j=0;j<GAMEBG_GRID_COLONM;j++){
        for(int i=0;i<(GAMEBG_GRID_ROW - j % 2);i++){
			GameMapRegion *r = gameMapRegion[j][i];
			r->changeArmy(Q_NULLPTR);
			r->changeOwner(0);
		}
    }
	mainGameMap->update();
	this->gameBegin();
}

int GameControler::createPlayer(bool i_isNPC, int id){
	if(id < 0){
		id = 1;
		bool findID = false;
		while(!findID){
            findID = true;
			for(int i=0;i<playerIDVList.size();i++){
				if(id == playerIDVList[i]){
					findID = false;
					id++;
					break;
				}
			}
		}
	}
	for(int i=0;i<playerIDVList.size();i++)
		if(id == playerIDVList[i])
			qDebug() << "repeat player id";
    Player *player = new Player(id, i_isNPC);
	playerIDVList.append(id);
    idPlayerMap.insert(id, player);
    player->disableInteraction();
	qDebug() << "create Player with id" << id;
}

void GameControler::removePlayer(Player *player){
	if(player == Q_NULLPTR){
		qDebug() << "GameCOntroler: player NULL";
		return;
	}
	if(actPlayer == Q_NULLPTR){
		qDebug() << "GameCOntroler: actPlayer NULL";
	}else if(actPlayer->getPlayerID() == player->getPlayerID()){
        actPlayer = Q_NULLPTR;
    }
	for(int i=0;i < player->getCityVList()->size();i++){
        player->getCityVList()->at(i)->changeOwner(0);
	}
	while(player->getArmyList()->size()){
		removeArmy(player->getArmyList()->at(0));
	}
	idPlayerMap.remove(player->getPlayerID());
    for(int i=0;i < playerIDVList.size();i++){
        if(player->getPlayerID() == playerIDVList[i]){
			playerIDVList.remove(i);
			break;
		}
	}
	delete player;
}

void GameControler::createArmy(int hexCoorX, int hexCoorY, int ownerID, int number, bool hasgeneral){
    if(!GameUtil::inMap(hexCoorX, hexCoorY)){
        qDebug() << "Trying to create a outmap army!";
        return;
    }
    Army *army1 = new Army(hexCoorX,hexCoorY, number ,ownerID, hasgeneral); //
    QList<ArmyAccessibleRegion *> list = army1->getArmyAccRgList();
    for(int i=0;i<list.size();i++){
        connect(list.at(i),SIGNAL(movingSignal(QGraphicsItem*,int,int)),
                this, SLOT(tryMoveArmy(QGraphicsItem*,int,int)));
    }
	connect(army1->getArmyCurrRg(), SIGNAL(armyClicked(QGraphicsItem*, bool)),
                this, SLOT(armyClicked(QGraphicsItem*, bool)));
    //Add to Map
    gameMapRegion[hexCoorX][hexCoorY]->changeArmy(army1);
    //Add to owner's list
    idPlayerMap[ownerID]->addArmy(army1);
    army1->disableInteraction();
    mainGameMap->addItem(army1);
}

void GameControler::removeArmy(Army *army){
    gameMapRegion[army->getHexCoorX()][army->getHexCoorY()]->changeArmy(Q_NULLPTR);
	mainGameMap->removeItem(army);
    idPlayerMap[army->getOwnerID()]->getArmyList()->removeAll(army);
    armyVList.removeAll(army);
	//
	delete army;
}	

void GameControler::armyClicked(QGraphicsItem *i_army, bool state){
    Army *army;
    if(typeid(*i_army) == typeid(Army))
        army = (Army *)i_army;
    qDebug()<<"one army clicked";
	bool inerObstruct[6];//iner (0--5) region has army or city
	GameMapRegion *armyRegion = gameMapRegion[army->getHexCoorX()][army->getHexCoorY()];
    //Test the accessible region
    QList<ArmyAccessibleRegion *> accRgList = army->getArmyAccRgList();
    for(int i=0;i<18;i++){
        intCoor hexCoor = GameUtil::getDesMapHexCoor(army->getHexCoorX(), army->getHexCoorY(),
                                                     GameUtil::nearbyHexCoor[i][0], GameUtil::nearbyHexCoor[i][1]);
		//Show accecable regions
        if(!GameUtil::inMap(hexCoor.first, hexCoor.second))
            accRgList.at(i)->setVisible(false);
		else{
			GameMapRegion *desRegion = gameMapRegion[hexCoor.first][hexCoor.second];
			//Analyse landform
			bool sameLandForm;
			if(armyRegion->getLandForm() == desRegion->getLandForm())
				sameLandForm = true;
			else if(armyRegion->getLandForm() != GameUtil::ocean &&
					desRegion->getLandForm() != GameUtil::ocean)
				sameLandForm = true;
			else 
				sameLandForm = false;

			if(i < 6){// iner circle rang = 1
				accRgList.at(i)->setVisible(true);
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
					accRgList.at(i)->setVisible(false);
				else
					accRgList.at(i)->setVisible(true);
			}
		}
    }
	//(in)activate other army
    QVector<Army *> *armyList = this->actPlayer->getArmyList();
    for(int i=0;i<armyList->size();i++){
		if(state){  //one army be activated
            armyList->at(i)->disableInteraction();
			army->enableInteraction();
		} else{
            armyList->at(i)->enableInteraction();
		}
	}
	mainGameMap->update();
	
}

void GameControler::moveArmy(Army *army, GameMapRegion *r){
    gameMapRegion[army->getHexCoorX()][army->getHexCoorY()]->changeArmy(Q_NULLPTR);
    dCoor centerPos = GameUtil::hexgonCenterPostion(r->getHexCoorX(), r->getHexCoorY());
    //move army container
    army->setPos(centerPos.first, centerPos.second);
    army->changeHexCoor(r->getHexCoorX(),r->getHexCoorY());
    r->changeArmy(army);
    if(r->getOwnerID() != army->getOwnerID() && r->getLandForm() != GameUtil::ocean){
		//occupied a city or defeat a army
		if(r->getLandForm() == GameUtil::city){
			//if region is a city, change Player's cityList
			if(r->getOwnerID() != 0){
                idPlayerMap[r->getOwnerID()]->getCityVList()->removeAll(r);
			}
            idPlayerMap[army->getOwnerID()]->getCityVList()->append(r);
		}
	//	if(r->getOwnerID() !=0 ){//defeat an army
	//	//	if(playerFail(idPlayerMap[r->getOwnerID()])){
	//				removePlayer(idPlayerMap[r->getOwnerID()]);
	//				tryEndGame();
	//		}
	//	}
        r->changeOwner(army->getOwnerID());
		//expend teh border
        for(int i=0;i<6;i++){
            intCoor nearHexCoor = GameUtil::getDesMapHexCoor(r->getHexCoorX(),r->getHexCoorY(),
                                                             GameUtil::nearbyHexCoor[i][0],
                                                             GameUtil::nearbyHexCoor[i][1]);
            if(GameUtil::inMap(nearHexCoor.first,nearHexCoor.second)){
                GameMapRegion *nr = gameMapRegion[nearHexCoor.first][nearHexCoor.second];
                if(nr->getLandForm() == GameUtil::land &&
                        nr->getArmy() == Q_NULLPTR){
                    nr->changeOwner(army->getOwnerID());
                }
            }

        }
    }
    mainGameMap->update();
	//after moved
}

void GameControler::armyFight(Army *attacker, GameMapRegion *r){
	//1 for attacker, 2 for defender
	bool win_flag = false;
	bool lost_flag = false;
	GameMapRegion *armyRegion = gameMapRegion[attacker->getHexCoorX()][attacker->getHexCoorY()];
	double terrainCost = 0;
	int r1,r2;//army remain	
	double bonus1,bonus2;
	std::default_random_engine generator;
    std::normal_distribution<double> distribution(1.0,1.0);
	int n1 = attacker->getArmyNumber();
	int n2 = r->getArmy()->getArmyNumber();
    bonus1 = distribution(generator);
    bonus2 = distribution(generator);
    if(bonus1 <= 0.33) bonus1 = 0.33;
    if(bonus2 <= 0.33) bonus2 = 0.33;
	if(r->getLandForm() == GameUtil::city){
		terrainCost += 0.3;
	}
    if(armyRegion->getLandForm() == GameUtil::ocean &&
            r->getLandForm() != GameUtil::ocean){
		terrainCost +=0.2;
	}
	r1 = (int)(n1 - n2 * bonus2);
	r2 = (int)(n2 - n1 * bonus1 * (1 - terrainCost));
	qDebug() << "Attaker power:" << n1 <<" "<<bonus1 <<" "<<terrainCost;
	qDebug() << "Defender power:" << n2 <<" "<<bonus2;
	if(r1 <= 0 && r2 <= 0){
		//benefit defender
		r1 = 0;
		r2 = 1;
		lost_flag = true;
	} else if(r1 <= 0){
		r1 = 0;
		lost_flag = true;
	} else if(r2 <= 0){
		r2 = 0;
		win_flag = true;
	}

    qDebug() << "finish fighting";
	
	attacker->changeArmyNumber(r1);
	r->getArmy()->changeArmyNumber(r2);	
	//win_flag = true;
	if(win_flag){
		removeArmy(r->getArmy());
        //r->changeArmy(Q_NULLPTR);
		moveArmy(attacker, r);
	} else if(lost_flag){
		removeArmy(attacker);
	}	
}

void GameControler::armyRegoup(Army *army1, GameMapRegion *r){
	//from army1 to army2(in r)
	Army *army2 = r->getArmy();
	int n1 = army1->getArmyNumber();
	int n2 = army2->getArmyNumber();
	if(n1 + n2 <= MAX_ARMY_NUM){
		army2->changeArmyNumber(n1 + n2);
		removeArmy(army1);
	} else{
		army2->changeArmyNumber(MAX_ARMY_NUM);
		army1->changeArmyNumber(n1 - (MAX_ARMY_NUM - n2));
	}
}

void GameControler::tryMoveArmy(QGraphicsItem *i_army, int hexCoorX, int hexCoorY){
    Army *army;
	Player *armyPlayer;
	Player *opponent;
    if(typeid(*i_army) == typeid(Army))
        army = (Army *)i_army;
    army->moved = true;
    army->setZValue(ARMY_LAYER_UP);
    intCoor mapHexCoor = GameUtil::getDesMapHexCoor(army->getHexCoorX(), army->getHexCoorY(),
                                                    hexCoorX, hexCoorY);
    if(GameUtil::inMap(mapHexCoor.first, mapHexCoor.second)){
        GameMapRegion *r = gameMapRegion[mapHexCoor.first][mapHexCoor.second];
        qDebug() << "Moving to" <<mapHexCoor.first <<","<<mapHexCoor.second;
        qDebug() << "Verify Moving to"
                 <<r->getHexCoorX() <<","
                 <<r->getHexCoorY();
		if(r->getOwnerID() != 0){
			opponent = idPlayerMap[r->getOwnerID()];
		}
		armyPlayer = idPlayerMap[army->getOwnerID()];
        if(r->getArmy() == Q_NULLPTR){
            GameControler::moveArmy(army, r);
        } else if(army->getOwnerID() != r->getArmy()->getOwnerID()){
            GameControler::armyFight(army, r);
        } else if(army->getOwnerID() == r->getArmy()->getOwnerID()){
            GameControler::armyRegoup(army, r);
            r->getArmy()->moved = true;
        }
        if(playerFail(armyPlayer) ){
			removePlayer(armyPlayer);
			tryEndGame();
		}
        if(playerFail(opponent)){
			removePlayer(opponent);
			tryEndGame();
		}
    }
    //active other army
    if(actPlayer != Q_NULLPTR){
        qDebug()<< "aaaaaaaaaaaaaa";
        if(!actPlayer->isNPC()){
            QVector<Army *> *armyList = this->actPlayer->getArmyList();
            for(int i=0;i<armyList->size();i++){
                    armyList->at(i)->enableInteraction();
            }
        }
    }
//    qDebug()<< "hhhhhhhhhhhhh";
	mainGameMap->update();
//    qDebug()<< "HHHHHHHHHHHHH";
}

void GameControler::activatePlayer(Player *player){
    actPlayer = player;
    QVector<Army *> *armyList = player->getArmyList();
    for(int i=0;i<armyList->size();i++){
        armyList->at(i)->moved = false;
	}
	if(player->isNPC()){

	} else{
		player->enableInteraction();
    }
}


void GameControler::inactivatePlayer(){
    Player *player = actPlayer;
    actPlayer = Q_NULLPTR;
    if(player != Q_NULLPTR){
        if(player->isNPC()){

        } else{
            player->disableInteraction();
        }
    }
}
void GameControler::nextTurnBtnPushed(){
	onePlayerFinish();	
}
