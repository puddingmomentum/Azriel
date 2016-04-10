#ifndef PORT_H
#define PORT_H

#include <QString>
#include <QMap>

class Port {

private:
    int id;
    QString name;
    QString latitude;
    QString longitude;
    QChar gun;
    QMap<QString, int> commodities;

public:

    void setId(int i);
    void setName(QString n);
    int getId();
    QString getName();
    void setLongitude(QString);
    void setLatitude(QString);
    void setGun(QString);
    void setCommodity(QMap<QString, int>);
    QString getLongitude();
    QString getLatitude();
    QChar getGun();
    QMap<QString, int> getCommodity();
    Port();
//    Port& operator=(const Port &other) const;
    ~Port();


};


#endif // PORT_H
