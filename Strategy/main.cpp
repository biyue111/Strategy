#include "widget.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QLayout>
#include <QObject>
#include <QPushButton>

#include "GameBackGround.h"
#include "GameBackGround.cpp"
#include "GameUtil.h"
#include "gameinterface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    GameInterface *gameInterface;
    gameInterface= new GameInterface(&w);
    w.show();
    //w->show();
    //Widget w;
    //w.setLayout(layout);
    //w.show();

    return a.exec();
}