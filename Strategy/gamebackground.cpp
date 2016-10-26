#include "GameBackGround.h"
#include "GameUtil.h"

int getGameBackGround(QGraphicsScene *scene){

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
    return 0;
}
