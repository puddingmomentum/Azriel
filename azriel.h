#ifndef AZRIEL_H
#define AZRIEL_H

#include "controller.h"
#include "ui_azriel.h"

#include <memory>
#include <QString>
#include <QMainWindow>


namespace Ui {
class Azriel;
}

class Azriel : public QMainWindow
{
    Q_OBJECT

public:
    explicit Azriel(Controller *c);
    ~Azriel();
    void populate(Port*);
    void initialize();
    void flipReadOnly(QLineEdit*);

private slots:

    void on_comboBox_currentIndexChanged(int index);

    void on_b_New_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_cb_writeable_clicked();

    void on_b_Save_clicked();

    void on_b_Delete_clicked();

    void on_b_swap_clicked();

private:
    Ui::Azriel *ui;

protected:
    Controller *controller;
    QList<QLineEdit*> l_lineEdits;
    QMap<QString,QLabel*> l_labels;
    void updateCPort(Port*);
    QList<QLabel*> starLabels;
    void clearStarLabels();
    void calcRoute();
    QRegExpValidator *r_coord, *r_yn;
    QIntValidator *r_commod;
};

#endif // AZRIEL_H
