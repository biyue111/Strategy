#include "gamecontroler.h"

GameControler::GameControler(QWidget *w){
    //mainWidget = w;
    mainGameMap = new QGraphicsScene();
    QLayout * layout = new QVBoxLayout();

    nextTurnBtn = new QPushButton("Next Turn");
    connect(nextTurnBtn,SIGNAL(clicked()),
			this, SLOT(nextTurnBtnPushed()));	
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
    //create Players
    this->createPlayer(1,0);
    this->createPlayer(2,0);
    this->createArmy(1,1,1);
    this->createArmy(2,1,1);
    this->createArmy(1,2,2);
	this->activatePlayer(idPlayerMap[playerIDVList[0]]);
	actPlayerSequenceNumber = 0;
}
void GameControler::onePlayerFinish(){
	inactivatePlayer();
	//find next player
	actPlayerSequenceNumber++;
	if(actPlayerSequenceNumber >= playerIDVList.size()){
		this->endTurn();
		actPlayerSequenceNumber = 0;
	}
	Player *nextPlayer = idPlayerMap[playerIDVList[actPlayerSequenceNumber]];
	activatePlayer(nextPlayer);
}
void GameControler::endTurn(){
	//add army in every city
	//int armdyI = 8;
    for(int i=0;i<cityVList.size();i++){
        if(cityVList[i]->getOwnerID()!=0){
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
}

void GameControler::createPlayer(int id, bool i_isNPC){
    Player *player = new Player(id, i_isNPC);
	playerIDVList.append(id);
    idPlayerMap.insert(id, player);
    player->disableInteraction();
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
    //Add to owner's list UNFINISHED
    idPlayerMap[ownerID]->addArmy(army1);
    army1->disableInteraction();
    mainGameMap->addItem(army1);
}

void GameControler::removeArmy(Army *army){
    gameMapRegion[army->getHexCoorX()][army->getHexCoorY()]->changeArmy(Q_NULLPTR);
	mainGameMap->removeItem(army);
    idPlayerMap[army->getOwnerID()]->getArmyList()->removeAll(army);
	//
	delete army;
}	

void GameControler::armyClicked(QGraphicsItem *i_army, bool state){
    Army *army;
    if(typeid(*i_army) == typeid(Army))
        army = (Army *)i_army;
    qDebug()<<"one army clicked";

    //Test the accessible region
    QList<ArmyAccessibleRegion *> accRgList = army->getArmyAccRgList();
    for(int i=0;i<18;i++){
        intCoor hexCoor = GameUtil::getDesMapHexCoor(army->getHexCoorX(), army->getHexCoorY(),
                                                     GameUtil::nearbyHexCoor[i][0], GameUtil::nearbyHexCoor[i][1]);
        if(!GameUtil::inMap(hexCoor.first, hexCoor.second))
            accRgList.at(i)->setVisible(false);
        else if(gameMapRegion[hexCoor.first][hexCoor.second]->getLandForm() == GameUtil::ocean &&
                i >= 6)
            accRgList.at(i)->setVisible(false);
        else
            accRgList.at(i)->setVisible(true);
    }

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
        r->changeOwner(army->getOwnerID());
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
	bool win_flag = false;
	//
	//attacking Code
    qDebug() << "begin fighting";
	win_flag = true;
	if(win_flag){
		removeArmy(r->getArmy());
        //r->changeArmy(Q_NULLPTR);
		moveArmy(attacker, r);
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
        if(r->getArmy() == Q_NULLPTR){
            GameControler::moveArmy(army, r);
        } else if(army->getOwnerID() != r->getArmy()->getOwnerID()){
            GameControler::armyFight(army, r);
        } else if(army->getOwnerID() == r->getArmy()->getOwnerID()){
            GameControler::armyRegoup(army, r);
            r->getArmy()->moved = true;
        }
    }
    //active other army
    if(!actPlayer->isNPC())
    {
        QVector<Army *> *armyList = this->actPlayer->getArmyList();
        for(int i=0;i<armyList->size();i++){
                armyList->at(i)->enableInteraction();
        }
    }
	mainGameMap->update();
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
	if(player->isNPC()){

	} else{
		player->disableInteraction();
	}
}
void GameControler::nextTurnBtnPushed(){
	onePlayerFinish();	
}
