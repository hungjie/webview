#include "mainwindow.h"
#include <QApplication>

#include <QTextCodec>

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    QApplication a(argc, argv);
    MainWindow* w = MainWindow::Instance();

    w->show();
    int status = a.exec();

    //qDebug() << w->myCookie()->save();

    delete w;

    qDebug() << status;

    return status;
}
