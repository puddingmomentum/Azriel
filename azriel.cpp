#include "azriel.h"
#include "ui_azriel.h"
#include "newport.h"
#include <QMessageBox>

Azriel::Azriel(Controller *c) :
    QMainWindow(0),
    ui(new Ui::Azriel), controller(c)
{
    ui->setupUi(this);

    r_coord = (new QRegExpValidator(QRegExp("^[0-9]{1,3}[NEWS]{1}$", Qt::CaseSensitive, QRegExp::PatternSyntax::RegExp ), this));
    r_yn= (new QRegExpValidator(QRegExp("[YN]"), this));
    r_commod = (new QIntValidator(0, 99, this));

    ui->le_Latitude->setValidator(r_coord);
    ui->le_Latitude_2->setValidator(r_coord);
    ui->le_Longitude->setValidator(r_coord);
    ui->le_Longitude_2->setValidator(r_coord);
    ui->le_Gun_Training->setValidator(r_yn);
    ui->le_Food->setValidator(r_commod);
    ui->le_Food_2->setValidator(r_commod);
    ui->le_Cloth->setValidator(r_commod);
    ui->le_Cloth_2->setValidator(r_commod);
    ui->le_Minerals->setValidator(r_commod);
    ui->le_Minerals_2->setValidator(r_commod);
    ui->le_Wine->setValidator(r_commod);
    ui->le_Wine_2->setValidator(r_commod);
    ui->le_Grain->setValidator(r_commod);
    ui->le_Grain_2->setValidator(r_commod);
    ui->le_Timber->setValidator(r_commod);
    ui->le_Timber_2->setValidator(r_commod);
    ui->le_Paper->setValidator(r_commod);
    ui->le_Paper_2->setValidator(r_commod);
    ui->le_Leather->setValidator(r_commod);
    ui->le_Leather_2->setValidator(r_commod);

    l_labels.insert("Food",ui->label );
    l_labels.insert( "Cloth",ui->label_2 );
    l_labels.insert( "Minerals",ui->label_3 );
    l_labels.insert( "Wine",ui->label_4 );
    l_labels.insert( "Grain",ui->label_5  );
    l_labels.insert( "Timber",ui->label_6  );
    l_labels.insert( "Paper",ui->label_7 );
    l_labels.insert( "Leather",ui->label_8 );


}

Azriel::~Azriel()
{
    delete ui;
}

void Azriel::on_comboBox_currentIndexChanged(int index)
{
//        clearStarLabels();
        controller->updateInfo(ui);
        on_comboBox_2_currentIndexChanged(ui->comboBox_2->currentIndex());
//        calcRoute();
}

void Azriel::on_b_New_clicked()
{
    NewPort *newP = new NewPort(0, controller);
    newP->exec();
    initialize();
}

void Azriel::initialize()
{
        QStringList portList = controller->getPortList();
        portList.sort(Qt::CaseSensitive);

        l_lineEdits = ui->portInfo->findChildren<QLineEdit*>();
        ui->comboBox->clear();
        ui->comboBox_2->clear();
        ui->comboBox->insertItems(0, portList);
        ui->comboBox_2->insertItem(0, "");
        ui->comboBox_2->insertItems(1, portList);
}

void Azriel::clearStarLabels()
{
    QMapIterator<QString, QLabel*> iter(l_labels);
    while (iter.hasNext())
    {
        iter.next();
        QLabel *l = iter.value();
        l->setText("");
    }
}

void Azriel::calcRoute()
{
    if (ui->comboBox_2->currentText() != "" ) {
        QString s = QString(ui->comboBox_2->currentText());
        Port *p = controller->getPort(s);
        Port *here = controller->getcPort();
        Route route = Route(*here, *p);
        float distance = route.getDistance();
        char d[50];
        sprintf(d, "%0.2f", distance);
        QString dist(d);
        ui->l_Distance_2->setText(dist);

        QMap<QString, int> com = p->getCommodity();
        char *b = route.getBearing();
        QString bearing = QString("%1").arg(b);

        ui->l_Bearing_2->setText(bearing);
        ui->le_Longitude_2->setText(p->getLongitude());
        ui->le_Latitude_2->setText(p->getLatitude());
        ui->le_Food_2->setText(QString::number(com.find("Food").value()));
        ui->le_Cloth_2->setText(QString::number(com.find("Cloth").value()));
        ui->le_Minerals_2->setText(QString::number(com.find("Minerals").value()));
        ui->le_Wine_2->setText(QString::number(com.find("Wine").value()));
        ui->le_Grain_2->setText(QString::number(com.find("Grain").value()));
        ui->le_Timber_2->setText(QString::number(com.find("Timber").value()));
        ui->le_Paper_2->setText(QString::number(com.find("Paper").value()));
        ui->le_Leather_2->setText(QString::number(com.find("Leather").value()));

        QMap<QString,int> commodity = route.getBestCommodity();
        QString best = commodity.firstKey();
        if ( best != "" )
        {
            l_labels[best]->setText("*");
            l_labels[best]->show();
        }
        float factor = route.getFactor();
        ui->l_factor->setText(QString("%1").arg(factor));

        delete p;
    } else
    {
        ui->l_Distance_2->setText("0");
        ui->le_Longitude_2->setText("0");
        ui->le_Latitude_2->setText("0");
        ui->le_Food_2->setText("0");
        ui->le_Cloth_2->setText("0");
        ui->le_Minerals_2->setText("0");
        ui->le_Wine_2->setText("0");
        ui->le_Grain_2->setText("0");
        ui->le_Timber_2->setText("0");
        ui->le_Paper_2->setText("0");
        ui->le_Leather_2->setText("0");
        ui->l_factor->setText("0");
    }
}

void Azriel::on_comboBox_2_currentIndexChanged(int index)
{
    clearStarLabels();
    calcRoute();

}

void Azriel::on_cb_writeable_clicked()
{
        flipReadOnly(ui->le_Latitude);
        flipReadOnly(ui->le_Longitude);
        flipReadOnly(ui->le_Gun_Training);
        flipReadOnly(ui->le_Food);
        flipReadOnly(ui->le_Cloth);
        flipReadOnly(ui->le_Minerals);
        flipReadOnly(ui->le_Wine);
        flipReadOnly(ui->le_Grain);
        flipReadOnly(ui->le_Timber);
        flipReadOnly(ui->le_Paper);
        flipReadOnly(ui->le_Leather);
        ui->b_Save->setEnabled(!ui->b_Save->isEnabled());
}

void Azriel::flipReadOnly(QLineEdit *obj)
{
    obj->setReadOnly(!obj->isReadOnly());
}

void Azriel::on_b_Save_clicked()
{
    Port *p = new Port();
    updateCPort(p);
    controller->save(p);
}

void Azriel::updateCPort(Port *p)
{
    p->setId( ui->l_port_id->text().toInt() );
    p->setName(ui->comboBox->currentText());
    p->setLatitude( ui->le_Latitude->text() );
    p->setLongitude( ui->le_Longitude->text() );
    p->setGun( ui->le_Gun_Training->text() );
    QMap<QString, int> com = p->getCommodity();
    com["Food"] = ui->le_Food->text().toInt();
    com["Cloth"] = ui->le_Cloth->text().toInt();
    com["Minerals"] = ui->le_Minerals->text().toInt();
    com["Wine"] = ui->le_Wine->text().toInt();
    com["Grain"] = ui->le_Grain->text().toInt();
    com["Paper"] = ui->le_Paper->text().toInt();
    com["Timber"] = ui->le_Timber->text().toInt();
    com["Leather"] = ui->le_Leather->text().toInt();
    p->setCommodity(com);
}

void Azriel::on_b_Delete_clicked()
{

    controller->removeMsg();
    initialize();
}

void Azriel::on_b_swap_clicked()
{
    if ( ui->comboBox_2->currentText() != "" )
    {
        int cOne = ui->comboBox->currentIndex();
        int cTwo = ui->comboBox_2->currentIndex();

        ui->comboBox->setCurrentIndex(cTwo-1);
        ui->comboBox_2->setCurrentIndex(cOne+1);
    }
}
