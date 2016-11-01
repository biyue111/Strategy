#ifndef GAMEUTIL_H
#define GAMEUTIL_H

/*Basic Parameter*/
#include <utility>
#include <math.h>
#include <QDebug>
#include <QPainter>
#define ROOT_3 1.732050808
#define HEXGON_SIDE_LENGTH 20
#define GAMEBG_GRID_COLONM 15
#define GAMEBG_GRID_ROW 10
#define SCENE_EGDE 10

typedef std::pair<int,int> intCoor;
typedef std::pair<double,double> dCoor;


class GameUtil
{
public:
    GameUtil(){}
    enum landform {land, ocean, city};

    static intCoor getRelativeHexCoor(int x,int y);
    static intCoor getDesMapHexCoor(int mapx, int mapy, int dx, int dy);
    static dCoor getPrecisePostion(int mapx,int mapy);
    static std::pair<double,double> getRectSize(int rowNum, int colNum);
    static dCoor hexgonOriginPostion(int hexCoorX, int hexCoorY);
    static dCoor hexgonCenterPostion(int hexCoorX, int hexCoorY);
    static bool accessible(int rang, int hexCoorX, int hexCoorY);
    static constexpr dCoor hexCenter = std::make_pair(0.5 * HEXGON_SIDE_LENGTH,
                                             0.5 * ROOT_3 * HEXGON_SIDE_LENGTH);
    static QColor getQColorbyID(int id);
    static bool inMap(int mapx, int mapy);
};

#endif // GAMEUTIL_H
