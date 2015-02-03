#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>
#include "mywebpage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    static MainWindow* Instance()
    {
        if(!owner_)
        {
            owner_ = new MainWindow();
        }

        return owner_;
    }

    static MyCookieJar* myCookie()
    {
        if(!mycookie_)
        {
            mycookie_ = new MyCookieJar();
            qDebug() << mycookie_->load();
        }

        return mycookie_;
    }

    TabWidget* tabWidget()
    {
        return t_;
    }

    QPoint viewPos();

    QLabel* statusLabel()
    {
        return msgLabel;
    }

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_actionHighlight_links_triggered(bool checked);

    void on_actionMove_triggered();

    void on_actionMovetoweb_triggered();

    void on_actionTimer_triggered();

    void on_actionGetcookie_triggered();

    void on_actionSavecookie_triggered();

    void on_actionClearcookie_triggered();

    void on_actionScroll_triggered();

    void on_actionStatus_triggered();

    void on_actionSetfirst_triggered();

    void on_actionSetsecond_triggered();

    void on_actionSetthird_triggered();

    void on_MainWindow_destroyed();

protected:
    void mouseMoveEvent(QMouseEvent *e);

private:
    Ui::MainWindow *ui;
    //WebView* v_;
    TabWidget* t_;
    QString jQuery;
    QLabel* msgLabel;

    static MainWindow* owner_;
    static MyCookieJar* mycookie_;
};

#endif // MAINWINDOW_H
