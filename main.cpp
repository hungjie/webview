#include "mainwindow.h"
#include <QApplication>

#include <QTextCodec>

#include <iostream>

#include "jsonencode.h"

int main(int argc, char *argv[])
{
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    if(argc != 2)
    {
        //todo
        return -1;
    }



    /**************************testing*******************************

    QString addr = "http://www.taobao.com";//argv[1];
    QString main_script = "var main_script = [ {\"func\":\"move\", \"parms\":{\"id\":\"J_TSearchForm\", \"offset\":{\"left\":10, \"top\":10}}}"
                          ", {\"func\":\"lbclick\", \"parms\":{}}"
                          ", {\"func\":\"timerinputvalue\", \"parms\":{\"cur_input\":\"\", \"time\":1000, \"input_array\":[\"\"], \"input_id\":\"q\"}}"
                          ", {\"func\":\"move\", \"parms\":{\"id_class\":{\"id\":\"J_TSearchForm\", \"class\":\"btn-search\"}, \"offset\":{\"left\":10, \"top\":10}}}"
                          ", {\"func\":\"lbclick\", \"parms\":{}}"
                          ", {\"func\":\"waitLoadFinished\",\"parms\":{\"times\":5, \"tab_index\":0}}"
                          ", {\"func\":\"forfunc\", \"parms\":{\"limit_times\":5, \"cur_times\":0, \"end_status\":false,\"end_condition\":\"foundelementid\", \"end_parms\":{\"id\":\"J_Itemlist_Pic_41581154377\",\"offset\":{}}, \"action_index\":0, \"actions\":[{\"func\":\"scroll\", \"parms\":{\"id_class\":{\"id\":\"mainsrp-pager\", \"class\":\"J_Ajax num icon-tag\", \"class_index\":-1},\"offset\":{\"left\":100, \"top\":100}}}, {\"func\":\"sleep\", \"parms\":{\"mtime\":2000}}, {\"func\":\"move\", \"parms\":{\"id_class\":{\"id\":\"mainsrp-pager\", \"class\":\"J_Ajax num icon-tag\", \"class_index\":-1} ,\"offset\":{\"left\":10, \"top\":10}}}, {\"func\":\"lbclick\", \"parms\":{\"id_class\":{\"id\":\"mainsrp-pager\", \"class\":\"J_Ajax num icon-tag\", \"class_index\":-1}, \"offset\":{\"left\":5, \"top\":5}}}, {\"func\":\"waitLoadFinished\",\"parms\":{\"times\":5, \"tab_index\":0}}]}}];";

    //****************************************************************/

    QString addr = "http://www.taobao.com";
    JsonEncode encode;
    encode.parser(argv[1]);
    QString main_script = encode.toJsonString();
    qDebug() << main_script;

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
