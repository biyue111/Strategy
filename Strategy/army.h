#ifndef ARMY_H
#define ARMY_H
#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QObject>
#include <QPainter>
#include <QList>
#include <typeinfo>
#include "GameUtil.h"

class ArmyCurrentRegion : public QObject, public QGraphicsItem
{
private:
    bool tryingToMove;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
public:
    ArmyCurrentRegion(QGraphicsItem *parent = Q_NULLPTR) :
        QGraphicsItem(parent){
        this->setPos(-GameUtil::hexCenter.first,-GameUtil::hexCenter.second);
        tryingToMove = false;
    }
    QRectF boundingRect() const
    {
        return QRectF(-HEXGON_SIDE_LENGTH / 2, 0,
                      HEXGON_SIDE_LENGTH * 2, ROOT_3 * HEXGON_SIDE_LENGTH);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget)
    {
        painter->setBrush(Qt::yellow);
        painter->drawPolygon(QPolygonF( QVector<QPointF>() <<
                                        QPointF( 0, 0 ) <<
                                        QPointF( HEXGON_SIDE_LENGTH, 0 ) <<
                                        QPointF( 1.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                        QPointF( HEXGON_SIDE_LENGTH, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                        QPointF( 0, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                        QPointF(-0.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH)));
    }
    void mousePressEventHelper(QGraphicsSceneMouseEvent * event);

};

class ArmyAccessibleRegion : public QObject,public QGraphicsItem
{
private:
    int hexCoorX,hexCoorY;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
public:
    ArmyAccessibleRegion(int i_hexCoorX, int i_hexCoorY, QGraphicsItem *parent = Q_NULLPTR) :
        QGraphicsItem(parent)
    {
        hexCoorX = i_hexCoorX;hexCoorY = i_hexCoorY;
        dCoor pos = GameUtil::hexgonOriginPostion(hexCoorX, hexCoorY);
        this->setPos(pos.first, pos.second);
    }
    QRectF boundingRect() const{
        return QRectF(-HEXGON_SIDE_LENGTH / 2, 0,
                      HEXGON_SIDE_LENGTH * 2, ROOT_3 * HEXGON_SIDE_LENGTH);
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        painter->setBrush(Qt::blue);
        painter->drawPolygon(QPolygonF( QVector<QPointF>() <<
                                        QPointF( 0, 0 ) <<
                                        QPointF( HEXGON_SIDE_LENGTH, 0 ) <<
                                        QPointF( 1.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                        QPointF( HEXGON_SIDE_LENGTH, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                        QPointF( 0, ROOT_3 * HEXGON_SIDE_LENGTH ) <<
                                        QPointF(-0.5 * HEXGON_SIDE_LENGTH, 0.5 * ROOT_3 * HEXGON_SIDE_LENGTH)));
    }
    int getHexCoorX(){return hexCoorX;}
    int getHexCoorY(){return hexCoorY;}
};

class ArmyAccessibleRegionContainer : public QObject,public QGraphicsItem
{
public:
    ArmyAccessibleRegionContainer(QGraphicsItem *parent = Q_NULLPTR) :
        QGraphicsItem(parent)
    {
        this->setPos(-GameUtil::hexCenter.first,-GameUtil::hexCenter.second);
    }
    QRectF boundingRect() const{
        qreal penWidth = 1;
        std::pair<double,double> size = GameUtil::getRectSize(5, 5);
        dCoor orgine = GameUtil::hexgonOriginPostion(2, 2);
        return QRectF(-orgine.first - 0.5 * HEXGON_SIDE_LENGTH, -orgine.second,
                      size.first, size.second);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget){
        std::pair<double,double> size = GameUtil::getRectSize(5, 5);
        dCoor orgine = GameUtil::hexgonOriginPostion(2, 2);
        painter->drawRect(-orgine.first - 0.5 * HEXGON_SIDE_LENGTH, -orgine.second,
                          size.first, size.second);
    }
};

class Army
{
private:
    int mobility;
    int armyNumber;
    int mapHexCoorX,mapHexCoorY;//Postion in the map
    bool haveGeneral;
    QGraphicsRectItem *armyContainer;
    QGraphicsRectItem *armyFigure;
    ArmyCurrentRegion *armyCurrentRegion;
    ArmyAccessibleRegionContainer *armyAccRegionCtn;
    QList<ArmyAccessibleRegion *> armyAccessibleRegionList;
    void moveTo();
public:
    //QGraphicsRectItem *armyTest;

    Army(int i_posX, int i_posY, int i_armyNumber, bool i_haveGeneral);
    QGraphicsRectItem *getArmyGUI() {return armyContainer;}
    ArmyCurrentRegion *getArmyhex() {return armyCurrentRegion;}
    int tryMoveTo(int posX,int posY);
};

#endif // ARMY_H
