#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private slots:
    void linkClicked(QUrl const& url);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    WebView* v_;
};

#endif // MAINWINDOW_H
