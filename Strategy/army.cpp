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
    dCoor centerPos = GameUtil::hexgonOriginPostion(hexCoorX, hexCoorY);
    this->parentItem()->parentItem()->moveBy(centerPos.first, centerPos.second);
}

Army::Army(int i_hexCoorX, int i_hexCoorY, int i_armyNumber, bool i_haveGeneral)
{
    mobility = 2;
    mapHexCoorX = i_hexCoorX;mapHexCoorY = i_hexCoorY;
    armyNumber = i_armyNumber;
    haveGeneral = i_haveGeneral;
    armyContainer = new QGraphicsRectItem(-5,-5,10,10);
    dCoor a = GameUtil::getPrecisePostion(i_hexCoorX,i_hexCoorY);
    armyContainer->setPos(a.first, a.second);

    armyFigure = new QGraphicsRectItem(-5,-5,10,10,armyContainer);
    //armyFigure->setPos(-GameUtil::hexCenter.first,-GameUtil::hexCenter.second);
    armyFigure->setZValue(2);//let the figure above region
    armyCurrentRegion = new ArmyCurrentRegion(armyContainer);
    armyAccRegionCtn = new ArmyAccessibleRegionContainer(armyContainer);
    for(int i = -mobility;i<=mobility;i++)
        for(int j=-mobility;j<=mobility;j++){
            if(GameUtil::accessible(2,i,j)){
                ArmyAccessibleRegion *aAR = new ArmyAccessibleRegion(i,j,armyAccRegionCtn);
                armyAccessibleRegionList.append(aAR);
            }
        }

    //armyTest = new QGraphicsRectItem(0,0,20,20,armyFigure);
}




