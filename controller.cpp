#include "ui_newport.h"
#include "port.h"
#include "controller.h"
#include <QString>
#include <QMessageBox>

Controller::Controller()
{

}

Controller::Controller(QString s) : pm(new PortManage(s))
{

}

Controller::~Controller()
{
    delete pm;
}

bool Controller::isConnected()
{
    return pm->isConnected();
}

void Controller::updateInfo(Ui::Azriel *ui)
{
    QString s = QString(ui->comboBox->currentText());
    Port *p = pm->getPort(s);
    cPort = *p;

    QMap<QString, int> com = p->getCommodity();

    ui->l_port_id->setText(QString::number(p->getId()));
    ui->le_Latitude->setText(p->getLatitude());
    ui->le_Longitude->setText(p->getLongitude());
    ui->le_Gun_Training->setText(QString(p->getGun()));
    ui->le_Food->setText(QString::number(com.find("Food").value()));
    ui->le_Cloth->setText(QString::number(com.find("Cloth").value()));
    ui->le_Minerals->setText(QString::number(com.find("Minerals").value()));
    ui->le_Wine->setText(QString::number(com.find("Wine").value()));
    ui->le_Grain->setText(QString::number(com.find("Grain").value()));
    ui->le_Timber->setText(QString::number(com.find("Timber").value()));
    ui->le_Paper->setText(QString::number(com.find("Paper").value()));
    ui->le_Leather->setText(QString::number(com.find("Leather").value()));

    delete p;
}

void Controller::save(Port *p)
{
    cPort = *p;
    pm->updatePort(cPort);
}

void Controller::remove()
{
    pm->deletePort(cPort);
}

void Controller::insert(Port p)
{
    pm->insertPort(p);
}

QStringList Controller::getPortList()
{
    return pm->getPortList();
}

Port* Controller::getPort(QString s)
{
    return pm->getPort(s);
}

Port* Controller::getcPort()
{
    return &cPort;
}

void Controller::removeMsg()
{
    QMessageBox *dlg = new QMessageBox(QMessageBox::Warning, "Delete Port", "Do you really want to delete this port?",
                                       QMessageBox::Yes | QMessageBox::No, this,
                                       Qt::MSWindowsFixedSizeDialogHint);

    dlg->connect(dlg->button(QMessageBox::Yes), SIGNAL(clicked()), this, SLOT(remove()));

    dlg->exec();

}
