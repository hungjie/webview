#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebFrame>
#include <QMessageBox>
#include <QErrorMessage>

#include <QCursor>
#include <QThread>

#include <QNetworkCookie>

MainWindow* MainWindow::owner_  = 0;
MyCookieJar* MainWindow::mycookie_ = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //v_ = new WebView(this);
    t_ = new TabWidget(this);
    t_->setAutoFillBackground(true);

    ui->verticalLayout->addWidget(t_);
    ui->verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);

    msgLabel = new QLabel();
    msgLabel->setMinimumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(msgLabel);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border:0px}"));//去掉label的边

    setMouseTracking(true);
    //v_->setStatusBarLable(msgLabel);
    t_->setMouseTracking(true);

    //QErrorMessage m(this);
    //m.showMessage(QString::number(v_->pos().x()) + "," + QString::number(v_->pos().y()));
    //m.exec();

    timer_ = new QTimer(this);
    timers_ = 0;

    connect(timer_, SIGNAL(timeout()), this, SLOT(checkWebFinished()));

    QFile file;
    file.setFileName(":/bin/script.js");
    file.open(QIODevice::ReadOnly);
    js_script_ = file.readAll();
    file.close();

    timer_->start(500);
}

MainWindow::~MainWindow()
{
    myCookie()->save();
    delete ui;
    delete t_;
    delete timer_;
    //delete mycookie_;
    QWebSettings::globalSettings()->clearIconDatabase();
    QWebSettings::globalSettings()->clearMemoryCaches();
}

QPoint MainWindow::viewPos()
{
    WebView* wv = MainWindow::Instance()->tabWidget()->currentWebView();
    if(wv)
    {
        QPoint p(0, 0);
        int window_x = wv->mapToGlobal(p).x();
        int window_y = wv->mapToGlobal(p).y();

        return QPoint(window_x, window_y);
    }

    return QPoint(0,0);
}

/*
void MainWindow::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        QUrl url(ui->lineEdit->text());
        //v_->loadUrl(url);
        t_->loadUrlInCurrentTab(url);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    t_->currentWebView()->back();
}
*/

void MainWindow::checkWebFinished()
{
    WebView* wv = MainWindow::Instance()->tabWidget()->currentWebView();
    if(wv && wv->webPage()->isLoadFinished())
    {
        timer_->stop();
        wv->webPage()->excuteJS("start()");
    }

    if(timers_++ == 100)
    {
        timer_->stop();
        QApplication::exit(-1);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    msgLabel->setText("("+QString::number(e->x())+","+QString::number(e->y())+")");
}
