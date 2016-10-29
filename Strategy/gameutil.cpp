#include "GameUtil.h"

intCoor GameUtil::getRelativeHexCoor(int x,int y){

}

intCoor GameUtil::getDesMapHexCoor(int mapx,int mapy, int x,int y){
    //Get the destination HexCoor. (x,y) is the relative HexCoor
}

dCoor GameUtil::getPrecisePostion(int mapx,int mapy){
    //Get the precise hexgon's center position in the scene from HexCoor
    double X = mapx * 1.5 * HEXGON_SIDE_LENGTH + 0.5 * HEXGON_SIDE_LENGTH;
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
