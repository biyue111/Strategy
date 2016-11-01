#include "gamecontroler.h"

GameControler::GameControler(QWidget *w){
    //mainWidget = w;
    mainGameMap = new QGraphicsScene();
    QLayout * layout = new QVBoxLayout();

    nextTurnBtn = new QPushButton("Next Turn");

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
    this->createArmy(1,1,1);

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

    for(int j=0;j<GAMEBG_GRID_COLONM;j++){
        QVector<GameMapRegion *> rv;
        for(int i=0;i<(GAMEBG_GRID_ROW - j % 2);i++){
            GameMapRegion *r = new GameMapRegion(j,i,GameUtil::land);
            rv.append(r);
            scene->addItem(r);
        }
        gameMapRegion.append(rv);
    }
    return ;
}

void GameControler::createArmy(int hexCoorX, int hexCoorY, int owner, bool hasgeneral){
    if(!GameUtil::inMap(hexCoorX, hexCoorY)){
        qDebug() << "Trying to create a outmap army!";
        return;
    }
    Army *army1 = new Army(hexCoorX,hexCoorY,1,owner, hasgeneral); //
    QList<ArmyAccessibleRegion *> list = army1->getArmyAccRgList();
    for(int i=0;i<list.size();i++){
        connect(list.at(i),SIGNAL(movingSignal(QGraphicsItem*,int,int)),
                this, SLOT(tryMoveArmy(QGraphicsItem*,int,int)));
    }
    mainGameMap->addItem(army1);
}

void GameControler::moveArmy(Army *army, GameMapRegion *r){
    gameMapRegion[army->getHexCoorX()][army->getHexCoorY()]->changeArmy(Q_NULLPTR);
    dCoor centerPos = GameUtil::hexgonCenterPostion(r->getHexCoorX(), r->getHexCoorY());
    //move army container
    army->setPos(centerPos.first, centerPos.second);
    army->changeHexCoor(r->getHexCoorX(),r->getHexCoorY());
    r->changeArmy(army);
    //if(r->getOwnerID() != army->getOwnerID()){
        r->changeOwner(army->getOwnerID());
    //}
    mainGameMap->update();
}

void GameControler::tryMoveArmy(QGraphicsItem *i_army, int hexCoorX, int hexCoorY){
    Army *army;
    if(typeid(*i_army) == typeid(Army))
        army = (Army *)i_army;
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
        }
    }
}
