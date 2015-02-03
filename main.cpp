#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = MainWindow::Instance();

    w->show();
    int status = a.exec();

    //qDebug() << w->myCookie()->save();

    delete w;

    qDebug() << status;

    return status;
}
