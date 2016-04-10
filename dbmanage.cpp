#include "dbmanage.h"

DBManage::DBManage(QString db) : lastPort(new Port())
{
    connect(db);
}

DBManage::~DBManage()
{
    delete lastPort;
}

QStringList DBManage::getPortList()
{
    QSqlQuery query;
    QString s = "SELECT port FROM Ports";
    QStringList portList;

    query.prepare(s);
    query.exec();

    while (query.next())
    {
        QSqlRecord rec = query.record();
        portList.append(rec.value(0).toString());
    }

    return portList;
}

Port* DBManage::sqlQuery(QVariant v)
{
    QSqlQuery query;

    switch (v.type())
    {
        case QMetaType::QString:
        {
            QString s = v.toString();
            s.prepend("SELECT * FROM Ports WHERE Port = '");
            s.append("'");
            query.prepare( s );
            break;
        }
        case QMetaType::Int:
        {
            QString s = QString("SELECT * FROM Ports WHERE id = %1").arg(v.toInt());
            query.prepare(s);
            break;
        }
        default:
        {
            return NULL;
        }
    }

    if ( !query.exec() )
    {
        return NULL;
    }
    else
    {
        query.next();
        QSqlRecord rec = query.record();
        lastPort->setId(rec.value(0).toInt());
        lastPort->setName(rec.value(1).toString());
        lastPort->setLongitude(rec.value(2).toString());
        lastPort->setLatitude(rec.value(3).toString());
        bool g = false;
        if ( rec.value(4).toString() == "Y" ) lastPort->setGun("Y");
        else lastPort->setGun("N");

    }

    QString s = QString("SELECT c.commodity, pcp.price "
              "FROM Ports_commodities_prices pcp "
              "LEFT JOIN Ports p ON pcp.port_fk = p.id "
              "LEFT JOIN commodities c ON pcp.commodity_fk = c.id "
              "WHERE port_fk = %1").arg(lastPort->getId());
    query.prepare(s);

    if ( !query.exec() )
    {
        return NULL;
    }
    else
    {
        QMap<QString, int> c = lastPort->getCommodity();    // get commodity list
        QMap<QString, int>::iterator iter;
        while (query.next()) {
            iter = c.find(query.record().value(0).toString());  // find commodity in list
            iter.value() = query.record().value(1).toInt();     // update it's value
        }
        lastPort->setCommodity(c);

    }
    Port *newPort = new Port(*lastPort);
    return newPort;
}

bool DBManage::insertPort(Port p)
{
    ledger.transaction();
    QSqlQuery query;
    bool sql_ok = true;

    if (  p.getName() != getPort(p.getName())->getName() )
    {
        QString s = QString("INSERT INTO Ports (ports, latitude, longitude, gun_training) "
                    "VALUES ( %1, %2, %3, %4 )").arg(p.getName()).arg(p.getLatitude()).arg(p.getLongitude()).arg(p.getGun());
        query.prepare(s);
        if (!query.exec()) {
            ledger.rollback();
            return false;
        } else {
            QMapIterator<QString,int> iter = p.getCommodity();
            while (iter.hasNext()) {
                iter.next();
                s = QString("SELECT id FROM commodities WHERE commodity = '%1'").arg(iter.key());
                query.prepare(s);
                if (!query.exec()) {
                    ledger.rollback();
                    return false;
                }
                int c_id = query.record().value(0).toInt();
                s = QString("INSERT INTO ports_commodities_prices (port_fk, commodity_fk, price) "
                    "VALUES ( %1, %2, %3 )").arg(p.getId()).arg(c_id).arg(iter.value());
                query.prepare(s);
                if (!query.exec()) {
                    ledger.rollback();
                    return false;
                }
            }
        }
    }
    ledger.commit();
    return true;
}

bool DBManage::updatePort(Port p)
{
    ledger.transaction();
    QSqlQuery query;
    QString s = QString("UPDATE Ports "
                "SET ( ports='%1', latitude='%2', longitude='%3', gun_training='%4' "
                "WHERE id=%5").arg(p.getName()).arg(p.getLatitude()).arg(p.getLongitude().arg(p.getGun()));
    query.prepare(s);
    if ( !query.exec() ) {
        qDebug() << "Update failed: " << query.lastError();
    } else {
        QMapIterator<QString,int> iter = p.getCommodity();
        while (iter.hasNext()) {
            iter.next();
            s = QString("SELECT id FROM commodities WHERE commodity = '%1'").arg(iter.key());
            query.prepare(s);
            if (!query.exec()) {
                ledger.rollback();
                return false;
            }
            int c_id = query.record().value(0).toInt();
            s = QString("UPDATE ports_commodities_prices "
                        "SET (price=%1) "
                        "WHERE port_fk=%2 AND commodity_fk=%2").arg(iter.value()).arg(p.getId()).arg(c_id);
            query.prepare(s);
            if (!query.exec()) {
                ledger.rollback();
                return false;
            }
        }
    }
    ledger.commit();
    return true;
}

void DBManage::deletePort(Port p)
{
    QSqlQuery query;
    QString s = QString("DELETE FROM ports WHERE id = %1").arg(p.getId());

    query.prepare(s);
    query.exec();

    s = QString("DELETE FROM ports_commodities_prices WHERE port_fk = %1").arg(p.getId());
    query.prepare(s);
    query.exec();
}

bool DBManage::setPort(Port p)
{

}

void DBManage::disconnect() {
    ledger.close();
}

bool DBManage::connect(QString db) {

    ledger = QSqlDatabase::addDatabase("QSQLITE");
    ledger.setDatabaseName(db);

    if ( !ledger.open() )
    {
        qDebug() << ledger.lastError();
        return false;
    } else
    {
        return true;
    }
}

bool DBManage::isConnected() {

    return ledger.open();
}
