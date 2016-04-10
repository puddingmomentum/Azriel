#include "route.h"
#include "port.h"
#include <math.h>
#include <QDebug>
#include <QGeoCoordinate>

Route::Route() {}

Route::Route(Port h, Port t) : here(h), there(t) {}


char* Route::getBearing()
{
    Point a = getPoints(here.getLatitude(), here.getLongitude());
    Point b = getPoints(there.getLatitude(), there.getLongitude());
    char d[2] = {};

    if ( sign(a.x) == sign(b.x) ) {
        switch (sign(b.x)) {
            case 1:
                d[0] = 'N';
                break;
            case 0:
                d[0] = ' ';
                break;
            case -1:
                d[0] = 'S';
                break;
        }
    } else {
        /* Should we go straight from point A to point B or go
         * around the globe the opposite way?
         * If the direct way is shorter, get the sign:
         * 1 is North, -1 is South (0 is treated as North for simplicity.
         * Add N to S to get direct distance, 180-N + 180 - S for indirect
         * abs because S will always be a minus number.*/
        int direct = 0, indirect = 0;
        direct = a.x + abs(b.x);
        indirect = (180 - a.x) + (180 - abs(b.x));

        if (direct <= indirect) {
            sign(b.x) ? d[0] = 'N' : d[0] = 'S';
        } else {
            sign(b.x) ? d[0] = 'S' : d[0] = 'N';
        }
    }

    if ( sign(a.y) == sign(b.y) ) {
        switch (sign(a.y)) {
            case 1:
                d[1] = 'E';
                break;
            case 0:
                d[1] = ' ';
                break;
            case -1:
                d[1] = 'W';
                break;
        }
    } else {
        /* Should we go straight from point A to point B or go
         * around the globe the opposite way?
         * If the direct way is shorter, get the sign:
         * 1 is East, -1 is West (0 is treated as East for simplicity.
         * Add E to W to get direct distance, 180-E + 180 - W for indirect
         * abs because W will always be a minus number.*/
        int direct = 0, indirect = 0;
        direct = a.y + abs(b.y);
        indirect = (180 - a.y) + (180 - abs(b.y));

        if (direct <= indirect) {
            sign(b.y) ? d[1] = 'E' : d[1] = 'W';
        } else {
            sign(b.y) ? d[1] = 'W' : d[1] = 'E';
        }
    }
 //   d[2] = ' ';
    char *p = new char[2];
    std::copy(d,d+2,p);
    p[2] = ' ';
    return p;
//    double phi1 = toRadians(a.x);
//    double phi2 = toRadians(b.x);
//    double lam1 = toRadians(a.y);
//    double lam2 = toRadians(b.y);

//    double deltaLat = log( tan( toRadians(b.x)/2 + M_PI/4 ) /
//                           tan( toRadians/2 + M_PI/4 ) );
//    double deltaLon = abs( toRadians(a.y) - toRadians(b.y) );
//    deltaLon = (deltaLon > 180 ) ? fmod(deltaLon, 180) : deltaLon;
//    return atan2(sin(lam2-lam1)*cos(phi2),
//                 cos(phi1)*sin(phi2) -
//                 sin(phi1)*cos(phi2)*cos(lam2-lam1)) * 180/M_PI;
}

float Route::getDistance()
{
    Point a = getPoints(here.getLatitude(), here.getLongitude());
    Point b = getPoints(there.getLatitude(), there.getLongitude());

    double latitudeArc  = (a.x - b.x) * DEG_TO_RAD;
    double longitudeArc = (a.y - b.y) * DEG_TO_RAD;
    double latitudeH = sin(latitudeArc * 0.5);
    latitudeH *= latitudeH;
    double lontitudeH = sin(longitudeArc * 0.5);
    lontitudeH *= lontitudeH;
    double tmp = cos(a.x*DEG_TO_RAD) * cos(b.x*DEG_TO_RAD);
    return 2.0 * asin(sqrt(latitudeH + tmp*lontitudeH));

}

double Route::toRadians(double x)
{
    return (x * M_PI) / 180;
}

Route::Point Route::getPoints(QString str1, QString str2)
{

    str1 = sign(str1);
    str2 = sign(str2);

    Point a;
    a.x = str1.toInt();
    a.y = str2.toInt();

    return a;
}

int Route::sign(double x)
{
    return ( 0 < x ) - ( x < 0);
}

QString Route::sign(QString s)
{
    QChar sign('-');

    QChar direction = s.data()[s.size()-1].toUpper();
    if ( direction == 'N' || direction == 'E' )
    {
        sign = '+';
    }
    s.chop(1);
    s = sign + s;

    return s;
}

QMap<QString,int> Route::getBestCommodity()
{
    QMap<QString, int> result;
    QMap<QString, int> h(here.getCommodity()), t(there.getCommodity());
    int best = 0, tmp=0;
//    QMapIterator<QString, int> iter(h);
    QMap<QString, int>::iterator iter = h.begin();

    while ( (iter != h.end()) ) // cycle through commodity prices
    {                           // determine what's profitable
        QString commodity = iter.key();
        if ( h[commodity] != 0 && t[commodity] != 0 && h[commodity] < t[commodity] )
        {
            tmp = t[commodity] - h[commodity];
            if ( tmp > best)
            {
                best=tmp;
                if ( !result.isEmpty() ) result.clear();
                result[iter.key()] = iter.value();
            }
        }
        iter++;
    }

    if ( result.isEmpty() )
    {
        result.insert("", 0);
    }

    return result;
}

float Route::getFactor()
{
    float distance = 0, profit = 0;
    float factor = 0;
    distance = getDistance();
    QMap<QString, int> best = getBestCommodity();
    QMap<QString,int> c = there.getCommodity();
    profit = c[best.firstKey()] - best.value(best.firstKey());
    if ( distance != 0 && profit != 0  )
    {
        factor = profit/distance;
    }
    return factor;
}

void Route::setSource(Port h)
{
    here = h;
}

void Route::setDest(Port t)
{
    there = t;
}

