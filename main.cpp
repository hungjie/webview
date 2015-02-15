#include "mainwindow.h"
#include <QApplication>

#include <QTextCodec>

#include <iostream>

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    QApplication a(argc, argv);

    if(argc != 3)
    {
        return -1;
    }

    QString addr = argv[1];
    QString main_script = argv[2];

    MainWindow* w = MainWindow::Instance();

    if(addr.isEmpty())
    {
        addr = "http://www.taobao.com";
    }

    w->load_addr(addr);
    w->main_script(main_script);

    w->show();

    QUrl url(w->load_addr());
    MainWindow::Instance()->tabWidget()->currentWebView()->loadUrl(url);

    int status = a.exec();

    //qDebug() << w->myCookie()->save();

    delete w;

    std::cout << status;

    return status;
}
