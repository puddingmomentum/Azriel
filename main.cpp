#include "azriel.h"
#include "ui_azriel.h"
#include "controller.h"

#include <QApplication>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller *controller = new Controller("/home/rob/Documents/Curse of Azriel/Ledger.db3");
    Azriel w(controller);

    w.show();

    w.initialize();

    int result = a.exec();

    delete controller;

    return result;
}
