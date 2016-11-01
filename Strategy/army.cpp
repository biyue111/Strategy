#include "army.h"

void ArmyCurrentRegion::mousePressEvent(QGraphicsSceneMouseEvent * event){
    QList<QGraphicsItem *> accessibleRegionList = this->parentItem()->childItems();
    for(int i=0;i<accessibleRegionList.size();i++){
        if(typeid(*(accessibleRegionList.at(i)))==typeid(ArmyAccessibleRegionContainer)){
            QGraphicsItem *ctn = accessibleRegionList.at(i);
            ctn->setVisible(!ctn->isVisible());
            break;
        }
    }
}

void ArmyAccessibleRegion::mousePressEvent(QGraphicsSceneMouseEvent * event){
    this->parentItem()->setVisible(false);
    //dCoor centerPos = GameUtil::hexgonOriginPostion(hexCoorX, hexCoorY);
    //move army container
    emit movingSignal(this->parentItem()->parentItem(),hexCoorX, hexCoorY);
    //this->parentItem()->parentItem()->moveBy(centerPos.first, centerPos.second);
}

Army::Army(int i_hexCoorX, int i_hexCoorY, int i_armyNumber, int i_ownerID, bool i_haveGeneral,
           QGraphicsItem *parent) : QGraphicsItem(parent)
{
    mobility = 2;
    mapHexCoorX = i_hexCoorX;mapHexCoorY = i_hexCoorY;
    armyNumber = i_armyNumber;
    ownerID = i_ownerID;
    haveGeneral = i_haveGeneral;
    //armyContainer = new QGraphicsRectItem(-5,-5,10,10);
    dCoor a = GameUtil::getPrecisePostion(i_hexCoorX,i_hexCoorY);
    this->setPos(a.first, a.second);

    armyFigure = new QGraphicsRectItem(-5,-5,10,10,this);
    //armyFigure->setPos(-GameUtil::hexCenter.first,-GameUtil::hexCenter.second);
    armyFigure->setZValue(2);//let the figure above region
    armyCurrentRegion = new ArmyCurrentRegion(this);
    armyAccRegionCtn = new ArmyAccessibleRegionContainer(this);
    for(int i = -mobility;i<=mobility;i++)
        for(int j=-mobility;j<=mobility;j++){
            if(GameUtil::accessible(2,i,j)){
                ArmyAccessibleRegion *aAR = new ArmyAccessibleRegion(i,j,armyAccRegionCtn);
                armyAccessibleRegionList.append(aAR);
            }
        }
    armyNumberText = new QGraphicsTextItem(QString::number(armyNumber),this);
    armyNumberText->setZValue(3);
    //armyTest = new QGraphicsRectItem(0,0,20,20,armyFigure);
}




