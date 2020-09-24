#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();    

    //w.GetCmd(false,"/usr/games/lutris","-lj");
    return a.exec();
}
