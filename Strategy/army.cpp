#include "army.h"

void ArmyCurrentRegion::mousePressEvent(QGraphicsSceneMouseEvent * event){
    QList<QGraphicsItem *> accessibleRegionList = this->parentItem()->childItems();
    for(int i=0;i<accessibleRegionList.size();i++){//find and change the armyAccRegionCtn
        if(typeid(*(accessibleRegionList.at(i)))==typeid(ArmyAccessibleRegionContainer)){
            QGraphicsItem *ctn = accessibleRegionList.at(i);
            if(!ctn->isVisible()){
                this->parentItem()->setZValue(ARMY_LAYER_DOWN);
            } else{
                this->parentItem()->setZValue(ARMY_LAYER_UP);
            }
			emit armyClicked(this->parentItem(), !ctn->isVisible());//send signal to gameController to en/dis other army
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
    moved = false;
    //armyContainer = new QGraphicsRectItem(-5,-5,10,10);
    dCoor a = GameUtil::getPrecisePostion(i_hexCoorX,i_hexCoorY);
    this->setPos(a.first, a.second);

    armyFigure = new QGraphicsRectItem(-8,-8,16,16,this);
    armyFigure->setBrush(GameUtil::getQColorbyID(ownerID));
    //armyFigure->setPos(-GameUtil::hexCenter.first,-GameUtil::hexCenter.second);
    armyFigure->setZValue(2);//let the figure above region
    armyCurrentRegion = new ArmyCurrentRegion(this);
    armyAccRegionCtn = new ArmyAccessibleRegionContainer(this);
	armyAccRegionCtn->setVisible(false);
//    for(int i = -mobility;i<=mobility;i++)
//        for(int j=-mobility;j<=mobility;j++){
//            if(GameUtil::accessible(2,i,j)){
//                ArmyAccessibleRegion *aAR = new ArmyAccessibleRegion(i,j,armyAccRegionCtn);
//                armyAccessibleRegionList.append(aAR);
//            }
//        }
	for(int i=0;i<18;i++){
        ArmyAccessibleRegion *aAR = new ArmyAccessibleRegion(GameUtil::nearbyHexCoor[i][0],
																GameUtil::nearbyHexCoor[i][1],armyAccRegionCtn);
    	armyAccessibleRegionList.append(aAR);
	}		
    armyNumberText = new QGraphicsTextItem(QString::number(armyNumber),this);
    QFont qfont = QFont();
    qfont.setBold(true);
    //armyNumberText->setFont(qfont);
    armyNumberText->setPos(-8,-10);
    armyNumberText->setZValue(3);
    //armyTest = new QGraphicsRectItem(0,0,20,20,armyFigure);
    this->setZValue(ARMY_LAYER_UP);
}

void Army::changeArmyNumber(int n){
    armyNumber = n;
    armyNumberText->setPlainText(QString::number(armyNumber));
}

void Army::enableInteraction(){
    if(!moved)
        this->armyCurrentRegion->setVisible(true);
    else
        this->disableInteraction();
}

void Army::disableInteraction(){
	this->armyAccRegionCtn->setVisible(false);
    this->armyCurrentRegion->setVisible(false);
}
