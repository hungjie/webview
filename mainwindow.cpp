#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebFrame>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    v_ = new WebView(this);
    ui->verticalLayout->addWidget(v_);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        QUrl url(ui->lineEdit->text());
        v_->loadUrl(url);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    v_->back();
}

void MainWindow::on_actionHighlight_links_triggered(bool checked)
{
    if(v_->progress() != 100)
    {
        QMessageBox error(this);
        error.setText("web finished not yet!");
        error.exec();

        return ;
    }

    QString code;
    if(!checked)
        code = "qt.jQuery('a').each( function () { qt.jQuery(this).css('background-color', 'yellow') } ); undefined";
    else
        code = "qt.jQuery('a').each( function () { qt.jQuery(this).css('background-color', 'null') } ); undefined";

    v_->page()->mainFrame()->evaluateJavaScript(code);
}
