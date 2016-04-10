#include "portmanage.h"

PortManage::PortManage(QString db) : lastPort(new Port())
{
    connect(db);
}

PortManage::~PortManage()
{
    delete lastPort;
}

QStringList PortManage::getPortList()
{
    QSqlQuery query(ledger);
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

Port* PortManage::sqlQuery(QVariant v)
{
    *lastPort = Port();
    QSqlQuery query(ledger);

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
        lastPort->setLatitude(rec.value(2).toString());
        lastPort->setLongitude(rec.value(3).toString());
        if ( rec.value(4) == "Y" ) lastPort->setGun(QChar('Y'));
        else lastPort->setGun(QChar('N'));

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
//        qDebug() << query.lastQuery();
        QMap<QString, int> c = lastPort->getCommodity();    // get commodity list
        QMap<QString, int>::iterator iter;
        while (query.next()) {
            if ( !query.record().isNull(0) )
            {
                iter = c.find(query.record().value(0).toString());  // find commodity in list
                iter.value() = query.record().value(1).toInt();     // update it's value
            }

        }
        lastPort->setCommodity(c);

    }
    Port *newPort = new Port(*lastPort);
    return newPort;
}

bool PortManage::insertPort(Port p)
{
    ledger.transaction();
    QSqlQuery query(ledger);
    QString currPort = getPort(p.getName())->getName();

    if (  p.getName() != currPort )
    {
        QString s = QString("INSERT INTO Ports (port, latitude, longitude, gun_training) "
                    "VALUES ( '%1', '%2', '%3', '%4' )").arg(p.getName()).arg(p.getLatitude()).arg(p.getLongitude()).arg(p.getGun());
        query.prepare(s);
        if (!query.exec()) {
            qDebug() << query.lastError() << "\n"
                   << query.lastQuery();
            ledger.rollback();
            return false;
        } else {
            ledger.commit();
            query.clear();
            insertNewCommodityRecords(p);
        }
            return true;
    }
    return false;
}

bool PortManage::insertNewCommodityRecords(Port p)
{
    QString s = QString("SELECT id FROM Ports WHERE port = '%1'").arg(p.getName());
    QSqlQuery query;
    query.prepare(s);
    query.exec();
    query.next();
    QSqlRecord rec = query.record();
    p.setId(rec.value(0).toInt());

    QMap<QString,int> com = p.getCommodity();
    QMapIterator<QString,int> iter = com;

    while (iter.hasNext()) {
        iter.next();
        s = QString("SELECT id FROM commodities WHERE commodity = '%1'").arg(iter.key());
        query.prepare(s);
        if (!query.exec()) {
            ledger.rollback();
            return false;
        }
        ledger.commit();
        qDebug() << query.executedQuery();
        query.next();
        rec = query.record();
        int c_id = rec.value(0).toInt();
        ledger.transaction();


        s = QString("INSERT INTO ports_commodities_prices (port_fk, commodity_fk, price) "
            "VALUES ( %1, %2, %3 )").arg(p.getId()).arg(c_id).arg(iter.value());
        query.prepare(s);
        if (!query.exec()) {
            ledger.rollback();
            return false;
        }
        ledger.commit();
        qDebug() << query.executedQuery();
    }
    return true;
}

bool PortManage::updatePort(Port p)
{

    // See if commodity records already exist for updating purposes
    QSqlQuery query(ledger);
    QString s = QString("SELECT id FROM ports_commodities_prices WHERE port_fk = %1").arg(p.getId());
    query.prepare(s);
    query.exec();

    query.next();
    QSqlRecord rec = query.record();
    int test = rec.value(0).toInt();
    qDebug() << test;

    // if records don't exit, create some w/ any data
    if ( !test ) {
        insertNewCommodityRecords(p);
    }

    ledger.transaction();

    // update port attributes
    s = QString("UPDATE Ports "
                "SET port='%1', latitude='%2', longitude='%3', gun_training='%4' "
                "WHERE id=%5").arg(p.getName()).arg(p.getLatitude()).arg(p.getLongitude()).arg(p.getGun()).arg(p.getId());
    query.prepare(s);
    if ( !query.exec() ) {
        qDebug() << "Update failed: " << query.lastError()
                 << query.lastQuery();
    } else {
        // update commodity prices for this port
        QMapIterator<QString,int> iter = p.getCommodity();
        while (iter.hasNext()) {
            iter.next();
            s = QString("SELECT id FROM commodities WHERE commodity = '%1'").arg(iter.key());
            query.prepare(s);
            if (!query.exec()) {
                ledger.rollback();
                return false;
            } else {
                ledger.commit();
            }
            query.next();
            QSqlRecord rec = query.record();
            QVariant retValue = rec.value(0);
            int c_id = rec.value(0).toInt();
            s = QString("UPDATE ports_commodities_prices "
                        "SET price=%1 "
                        "WHERE port_fk=%2 AND commodity_fk=%3").arg(iter.value()).arg(p.getId()).arg(c_id);
            query.prepare(s);
            if (!query.exec()) {
                ledger.rollback();
                return false;
            }
            qDebug() << query.executedQuery();
            ledger.commit();
        }
        return true;
    }
    return false;
}

void PortManage::deletePort(Port p)
{
    QSqlQuery query(ledger);
    QString s = QString("DELETE FROM ports WHERE id = %1").arg(p.getId());

    query.prepare(s);
    query.exec();

    s = QString("DELETE FROM ports_commodities_prices WHERE port_fk = %1").arg(p.getId());
    query.prepare(s);
    query.exec();
}

void PortManage::disconnect() {
    ledger.close();
}

bool PortManage::connect(QString db) {

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

bool PortManage::isConnected() {

    return ledger.open();
}
