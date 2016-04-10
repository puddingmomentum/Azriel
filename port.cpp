#include "port.h"

Port::Port() : id(0), name(), latitude(), longitude(), gun(false), commodities()
{
    commodities.insert("Food", 0);
    commodities.insert("Cloth", 0);
    commodities.insert("Minerals", 0);
    commodities.insert("Wine", 0);
    commodities.insert("Grain", 0);
    commodities.insert("Timber", 0);
    commodities.insert("Paper", 0);
    commodities.insert("Leather", 0);
}

Port::~Port() {}


void Port::setId(int i)
{
    id = i;
}

void Port::setName(QString n)
{
    name = n;
}

void Port::setLongitude(QString s) {
    longitude = s;
}

void Port::setLatitude(QString s) {
    latitude = s;
}

void Port::setGun(QString g) {
    gun = g.data()[0];
}

void Port::setCommodity(QMap<QString, int> c){
    commodities = c;
}

QString Port::getName()
{
    return name;
}

QString Port::getLongitude() {
    return longitude;
}

QString Port::getLatitude() {
    return latitude;
}

QChar Port::getGun()
{
    return gun;
}

QMap<QString, int> Port::getCommodity() {
    return commodities;
}

int Port::getId()
{
    return id;
}
