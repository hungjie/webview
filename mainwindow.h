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
    ~MainWindow();

    static MainWindow* Instance()
    {
        if(!owner_)
        {
            owner_ = new MainWindow();
        }

        return owner_;
    }

    TabWidget* tabWidget()
    {
        return t_;
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

protected:
    void mouseMoveEvent(QMouseEvent *e);

private:
    Ui::MainWindow *ui;
    WebView* v_;
    TabWidget* t_;
    QString jQuery;

    QLabel* msgLabel;

    static MainWindow* owner_;
};

#endif // MAINWINDOW_H
