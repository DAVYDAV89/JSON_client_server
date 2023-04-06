#include <QApplication>
#include <QFile>
#include <QDebug>
#include "Server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Server s;

    return a.exec();
}
