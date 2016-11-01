#include "gameinterface.h"
#include "army.h"

GameInterface::GameInterface(QWidget *w)
{
    //mainWidget = w;
    mainGameMap = new QGraphicsScene();
    QLayout * layout = new QVBoxLayout();
    Army *army1 = new Army(10,9,1,false); //

    nextTurnBtn = new QPushButton("Next Turn");

    double sceneX = (0.5 + 1.5 * GAMEBG_GRID_COLONM) * HEXGON_SIDE_LENGTH;
    double sceneY = GAMEBG_GRID_ROW * ROOT_3 * HEXGON_SIDE_LENGTH;
    mainGameMap->addRect(0,0,sceneX,sceneY);
    mainGameMap->setSceneRect(0,0,sceneX,sceneY);
    getGameBackGround(mainGameMap);

    //army1->getArmyFigure()->setPos(20,20);
    mainGameMap->addItem(army1);
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
