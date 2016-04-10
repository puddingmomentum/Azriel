#ifndef ROUTE_H
#define ROUTE_H

#include "port.h"
#include <QMap>
#include <QString>

class Route
{
private:
    Port here;
    Port there;
    struct Point {
        int x, y;
    };
    QString sign(QString);
    int sign(double);
    static constexpr double DEG_TO_RAD = 0.017453292519943295769236907684886;
    static constexpr double EARTH_RADIUS_IN_METERS = 6372797.560856;

public:
    Route();
    Route(Port h, Port t);
//    static constexpr enum class Directions : int { NORTH = 1, SOUTH = -1, EITHER = 0 };
    float getDistance();
    Point getPoints(QString, QString);
    QMap<QString,int> getBestCommodity();
    float getFactor();
    void setSource(Port);
    void setDest(Port);
    double toRadians(double);
    char* getBearing();

};

#endif // ROUTE_H
