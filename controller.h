#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "portmanage.h"
#include "ui_azriel.h"
#include <QMainWindow>
#include <memory>

class Controller : public QWidget
{
    Q_OBJECT

public:
    Controller();
    Controller(QString s);
    ~Controller();
    void updateInfo(Ui::Azriel *ui);
    void updateCPort(Port *cPort);
    void save(Port*);
    void insert(Port p);
    void removeMsg();
    QStringList getPortList();
    Port* getPort(QString);
    Port* getcPort();
    bool isConnected();

protected:
    PortManage *pm;
    Port cPort;

public slots:
    void remove();

};

#endif // CONTROLLER_H
