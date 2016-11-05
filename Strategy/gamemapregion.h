#ifndef GAMEMAPREGION_H
#define GAMEMAPREGION_H

#include <QGraphicsItem>
#include <QPainter>
#include "army.h"

class GameMapRegion : public QGraphicsItem, public QObject
{
private:
    int mapHexCoorX,mapHexCoorY;
    int ownerID;
    Army *inRegionArmy;
    GameUtil::landform landform;

public:
    GameMapRegion(int i_hexCoorX, int i_hexCoorY, GameUtil::landform i_landform,
                  QGraphicsItem *parent = Q_NULLPTR) :
        QGraphicsItem(parent)
    {
        mapHexCoorX = i_hexCoorX;mapHexCoorY = i_hexCoorY;
        landform = i_landform;
        ownerID = 0;
        inRegionArmy = Q_NULLPTR;
        dCoor pos = GameUtil::hexgonOriginPostion(mapHexCoorX, mapHexCoorY);
        this->setPos(pos.first, pos.second);
    }
    QRectF boundingRect() const{
        return QRectF(-HEXGON_SIDE_LENGTH / 2, 0,
                      HEXGON_SIDE_LENGTH * 2, ROOT_3 * HEXGON_SIDE_LENGTH);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        QBrush b;
        if(ownerID == 0)
            //b = QBrush(Qt::NoBrush);
            painter->setBrush(Qt::NoBrush);
        else{
            qDebug()<<"OwnerID"<<ownerID;
            b = QBrush(GameUtil::getQColorbyID(ownerID),Qt::DiagCrossPattern);
            painter->setBrush(b);
        }
        //painter->setBrush(Qt::red);
        painter->drawPolygon(QPolygonF( QVector<QPointF>() <<
                                        QPointF( 0, 0 ) <<
                                        QPointF( HEXGON_SIDE_LENGTH, 0 ) <<
                                        QPointF( 1.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                        QPointF( HEXGON_SIDE_LENGTH, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                        QPointF( 0, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                        QPointF(-0.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH)));
    }

    changeOwner(int id) {ownerID = id;}
    changeArmy(Army *army) {inRegionArmy = army;}
    Army *getArmy() {return inRegionArmy;}
    int getOwnerID() {return ownerID;}
    int getHexCoorX(){return mapHexCoorX;}
    int getHexCoorY(){return mapHexCoorY;}
	GameUtil::landform getLandForm(){return landform;}
};

#endif // GAMEMAPREGION_H
