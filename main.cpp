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
        //todo
        //return -1;
    }

    //**************************testing*******************************
    QString addr = "http://www.baidu.com";//argv[1];
    QString main_script = "var main_script = [ {\"func\":\"move\", \"parms\":{\"id\":\"kw\"}}"
            ", {\"func\":\"lbclick\", \"parms\":{}}"
            ", {\"func\":\"timerinputvalue\", \"parms\":{\"cur_input\":\"\", \"time\":1000, \"input_array\":[\"L\",\"O\", \"V\", \"E\"], \"input_id\":\"kw\"}}"
            ", {\"func\":\"move\", \"parms\":{\"id\":\"su\"}}, {\"func\":\"lbclick\", \"parms\":{\"id\":\"su\", \"offset\":{\"left\":5, \"top\":5}}}"
            ", {\"func\":\"waitLoadFinished\",\"parms\":{\"times\":5, \"tab_index\":0}}"
            ", {\"func\":\"forfunc\", \"parms\":{\"limit_times\":5, \"cur_times\":0, \"end_status\":false,\"end_condition\":\"foundelementid\", \"end_parms\":{\"id\":\"23\",\"offset\":{}}, \"action_index\":0, \"actions\":[{\"func\":\"scroll\", \"parms\":{\"id_class\":{\"id\":\"page\", \"class\":\"n\", \"class_index\":-1}}}, {\"func\":\"move\", \"parms\":{\"id_class\":{\"id\":\"page\", \"class\":\"n\", \"class_index\":-1}}}, {\"func\":\"lbclick\", \"parms\":{\"id_class\":{\"id\":\"page\", \"class\":\"n\", \"class_index\":-1}}}, {\"func\":\"waitLoadFinished\",\"parms\":{\"times\":5, \"tab_index\":0}}]}}];";//argv[2];
    //****************************************************************

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
