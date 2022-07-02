#include "mainwindow.h"

#include <QApplication>
#include <modeling.h>
#include <controller.h>
#include <QVector>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Controller *c = new Controller(w);

    w.show();

    return a.exec();
}
