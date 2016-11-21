#include "GameUtil.h"

const int GameUtil::nearbyHexCoor[18][2]= {
       //in Relative hexCoor
       {0,-1},{1,-1},{1,0},{0,1},{-1,0},{-1,-1},
       {0,-2},{1,-2},{2,-1},{2,0},{2,1},{1,1},{0,2},{-1,1},{-2,1},{-2,0},{-2,-1},{-1,-2}
};

intCoor GameUtil::getRelativeHexCoor(int x,int y){

}

intCoor GameUtil::getDesMapHexCoor(int mapx,int mapy, int dx,int dy){
    //Get the destination HexCoor. (dx,dy) is the destination in relative HexCoor
    //(mapx,mapy) is the map(absolu) hexCoor
    //qDebug() << "GameUtil::getDesMapHexCoor"<<mapx <<","<<mapy<<"|"
    //         << dx<<","<<dy;
    if(mapx % 2 == 0)
        return std::make_pair(mapx + dx, mapy + dy);
    else if(dx % 2 == 0)
        return std::make_pair(mapx + dx, mapy + dy);
    else
        return std::make_pair(mapx + dx, mapy + dy + 1);
}

bool GameUtil::inMap(int mapx,int mapy){
    if(mapx < 0 || mapy < 0)
        return false;
    else if(mapx >= GAMEBG_GRID_COLONM || mapy >= GAMEBG_GRID_ROW - mapx % 2)
        return false;
    else
        return true;
}

dCoor GameUtil::getPrecisePostion(int mapx,int mapy){
    //Get the precise hexgon's center position in the scene from HexCoor
    double X = mapx * 1.5 * HEXGON_SIDE_LENGTH;
    double Y = (mapy + (double)(mapx % 2) / 2) * ROOT_3 * HEXGON_SIDE_LENGTH;
    return std::make_pair(X+ GameUtil::hexCenter.first,
                          Y+ GameUtil::hexCenter.second);
}

std::pair<double,double> GameUtil::getRectSize(int rowNum, int colNum){
    //row number == maxY, col Number == maxX
    if (colNum == 0)
        return std::make_pair(0,
                              rowNum * ROOT_3 * HEXGON_SIDE_LENGTH);
    else
    return std::make_pair((0.5 + colNum * 1.5) * HEXGON_SIDE_LENGTH,
                          rowNum * ROOT_3 * HEXGON_SIDE_LENGTH);
}

dCoor GameUtil::hexgonOriginPostion(int hexCoorX, int hexCoorY){
    //in Relative Hex Coor (double)(j % 2) / 2
    return std::make_pair(hexCoorX * 1.5 * HEXGON_SIDE_LENGTH,
                         (hexCoorY + (double)(abs(hexCoorX) % 2) / 2) * ROOT_3 * HEXGON_SIDE_LENGTH);
}

dCoor GameUtil::hexgonCenterPostion(int hexCoorX, int hexCoorY){
    dCoor a = GameUtil::hexgonOriginPostion(hexCoorX,hexCoorY);
    a.first = a.first + GameUtil::hexCenter.first;
    a.second = a.second + GameUtil::hexCenter.second;
    return a;
}

bool GameUtil::accessible(int rang, int hexCoorX, int hexCoorY){
    //verfy rang <= 2
    //in relative hex Coor,
    //whether the point is in a "circle" with rayon rang
    int x = abs(hexCoorX), y = hexCoorY;
    if(x == 0 && y ==0)
        return false;
    else if(x + abs(y) <= rang)
        return true;
    else if(x == 1 && y == -rang)
        return true;
    else if(x == rang && abs(y) == 1)
        return true;
    else
        return false;
}

QColor GameUtil::getQColorbyID(int id){
    switch (id){
    case 0:
        return Qt::black;
        break;
    case 1:
        return Qt::red;
        break;
    case 2:
        return Qt::cyan;
        break;
    case 3:
        return Qt::yellow;
        break;
    case 4:
        return Qt::magenta;
        break;
    case 5:
        return Qt::white;
        break;
    case 6:
        return Qt::green;
        break;
    }
}

QString GameUtil::getNamebyID(int id){
    switch (id){
    case 0:
        return QString("No");
        break;
    case 1:
        return QString("Red");
        break;
    case 2:
        return QString("Cyan");
        break;
    case 3:
        return QString("Yellow");
        break;
    case 4:
        return QString("Magenta");
        break;
    case 5:
        return QString("White");
        break;
    case 6:
        return QString("Green");
        break;
    }
}

/*
QString GameUtil::getNamebyID(int id){
    switch (id){
    case 0:
        return Qt::black;
        break;
    case 1:
        return Qt::red;
        break;
    case 2:
        return Qt::cyan;
        break;
    case 3:
        return Qt::yellow;
        break;
    case 4:
        return Qt::magenta;
        break;
    case 5:
        return Qt::white;
        break;
    case 6:
        return Qt::green;
        break;
    }
}
*/
QColor GameUtil::getQColorbyLandForm(GameUtil::landform l){
    switch (l){
	case GameUtil::land:
        return Qt::transparent;
        break;
	case GameUtil::ocean:
        return Qt::blue;
        break;
	case GameUtil::city:
        return Qt::darkGray;
        break;
    }
	
}
