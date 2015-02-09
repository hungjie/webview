#include "mainwindow.h"
#include <QApplication>

#include <QTextCodec>

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    QApplication a(argc, argv);
    MainWindow* w = MainWindow::Instance();
    w->show();

    QUrl url("http://www.baidu.com");
    MainWindow::Instance()->tabWidget()->currentWebView()->loadUrl(url);

    int status = a.exec();

    //qDebug() << w->myCookie()->save();

    delete w;

    qDebug() << status;

    return status;
}
