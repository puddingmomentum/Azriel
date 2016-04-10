#ifndef PortManage_H
#define PortManage_H

#include "port.h"
#include "route.h"
#include <QStringList>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>


class PortManage
{
private:
    QSqlDatabase ledger;
    unsigned int id;
    QString pName;
    Port *lastPort;


public:
    PortManage(QString db);
    ~PortManage();
    bool connect(QString db);
    bool isConnected();
    QStringList getPortList();
    Port *sqlQuery(QVariant);
    bool setPort(Port p);
    void disconnect();
    bool insertPort(Port);
    bool updatePort(Port);
    void deletePort(Port);

    template <class T> Port* getPort(T p)
    {

        Port* port = sqlQuery( QVariant(p) );
        return port;
    }
};

#endif // PortManage_H
