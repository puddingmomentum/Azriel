#ifndef NEWPORT_H
#define NEWPORT_H

#include "controller.h"
#include <QRegExpValidator>
#include <memory>
#include <QDialog>

namespace Ui {
class NewPort;
}

class NewPort : public QDialog
{
    Q_OBJECT

public:
//    explicit NewPort(QWidget *parent = 0);
    NewPort(QWidget *parent, Controller*);
    ~NewPort();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewPort *ui;
    Controller *controller;
    QRegExpValidator *rxp;
    QRegExpValidator *rxv;
    QRegExpValidator *rxiv;
    QIntValidator *rintv;
};

#endif // NEWPORT_H
