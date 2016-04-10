#include "newport.h"
#include "ui_newport.h"

//NewPort::NewPort(QWidget *parent) :
//    QDialog(parent),
//    ui(new Ui::NewPort)

//{
//    ui->setupUi(this);
//}

NewPort::NewPort(QWidget *parent, Controller *c) :
    QDialog(parent),
    ui(new Ui::NewPort),
    controller(c),
    rxp(new QRegExpValidator(QRegExp("[A-Za-z ]{0,20}"))),
    rxv(new QRegExpValidator(QRegExp("^[0-9]{1,3}[NEWS]{1}$", Qt::CaseSensitive, QRegExp::PatternSyntax::RegExp ), this)),
    rxiv(new QRegExpValidator(QRegExp("[YN]"), this)),
    rintv(new QIntValidator(0, 99, this))
{
    ui->setupUi(this);
    ui->le_PortName->setValidator(rxp);
    ui->le_Latitude->setValidator(rxv);
    ui->le_Longitude->setValidator(rxv);
    ui->le_Gun_Training->setValidator(rxiv);
    ui->le_Food->setValidator(rintv);
    ui->le_Cloth->setValidator(rintv);
    ui->le_Minerals->setValidator(rintv);
    ui->le_Wine->setValidator(rintv);
    ui->le_Grain->setValidator(rintv);
    ui->le_Timber->setValidator(rintv);
    ui->le_Paper->setValidator(rintv);
    ui->le_Leather->setValidator(rintv);

    ui->le_Food->setText("0");
    ui->le_Cloth->setText("0");
    ui->le_Minerals->setText("0");
    ui->le_Wine->setText("0");
    ui->le_Grain->setText("0");
    ui->le_Timber->setText("0");
    ui->le_Paper->setText("0");
    ui->le_Leather->setText("0");

}

NewPort::~NewPort()
{
    delete rxp;
    delete rxv;
    delete rxiv;
    delete rintv;
    delete ui;
}

void NewPort::on_buttonBox_accepted()
{
    Port p;

    p.setName(ui->le_PortName->text());
    p.setLatitude(ui->le_Latitude->text());
    p.setLongitude(ui->le_Longitude->text());
    p.setGun(QChar(ui->le_Gun_Training->text().data()[0]));

    QMap<QString,int> c = p.getCommodity();
    QMapIterator<QString,int> iter(c);


    c["Food"] = ui->le_Food->text().toInt();
    c["Cloth"] = ui->le_Cloth->text().toInt();
    c["Minerals"] = ui->le_Minerals->text().toInt();
    c["Wine"] = ui->le_Wine->text().toInt();
    c["Grain"] = ui->le_Grain->text().toInt();
    c["Timber"] = ui->le_Timber->text().toInt();
    c["Paper"] = ui->le_Paper->text().toInt();
    c["Leather"] = ui->le_Leather->text().toInt();

    p.setCommodity(c);

    controller->insert(p);
}
