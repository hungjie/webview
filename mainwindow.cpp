#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWebFrame>
#include <QMessageBox>
#include <QErrorMessage>

#include <QCursor>
#include <QThread>

#include <QNetworkCookie>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    v_ = new WebView(this);
    ui->verticalLayout->addWidget(v_);
    ui->verticalLayout->setSizeConstraint(QLayout::SetMaximumSize);

    msgLabel = new QLabel();
    msgLabel->setMinimumSize(msgLabel->sizeHint());
    msgLabel->setAlignment(Qt::AlignHCenter);
    statusBar()->addWidget(msgLabel);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border:0px}"));//去掉label的边

    setMouseTracking(true);
    v_->setStatusBarLable(msgLabel);
    v_->setMouseTracking(true);

    QErrorMessage m(this);
    m.showMessage(QString::number(v_->pos().x()) + "," + QString::number(v_->pos().y()));
    m.exec();
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
        code = "qt.jQuery('a').each( function () { qt.jQuery(this).css('background-color', 'yellow') } ); ";
    else
        code = "qt.jQuery('a').each( function () { qt.jQuery(this).css('background-color', 'null') } ); ";

    v_->webPage()->mainFrame()->evaluateJavaScript(code);
}

void MainWindow::on_actionMove_triggered()
{
    //QCursor::setPos(0,0);
    QPoint p = v_->pos();

    //p = v_->mapFromParent(p);

    p = this->mapToGlobal(p);

    QCursor::setPos(p);
}

void MainWindow::on_actionMovetoweb_triggered()
{
    //QPoint p = this->mapFromGlobal(this->pos());
    QPoint p = this->pos();
    p.setX(this->width() + p.x());
    p.setY(this->height() + p.y());
    QCursor::setPos(p);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    msgLabel->setText("("+QString::number(e->x())+","+QString::number(e->y())+")");
}

void MainWindow::on_actionTimer_triggered()
{
    QPoint p = v_->pos();

    //p = v_->mapFromParent(p);

    p = this->mapToGlobal(p);

    v_->webPage()->moveMouse(p.x(),p.y());
}

void MainWindow::on_actionGetcookie_triggered()
{
    QNetworkCookie cookie;
    QList<QNetworkCookie> list;
    list = v_->myCookie()->mycookies();
    //list = v_->myCookie()->cookieByUrl("http://www.baidu.com");

    foreach (cookie ,list)
    {
        qDebug()<< "---------------------";
        qDebug()<< cookie.name();
        qDebug()<< cookie.value();
    }
}

void MainWindow::on_actionSavecookie_triggered()
{
    qDebug() << v_->myCookie()->save();
}

void MainWindow::on_actionClearcookie_triggered()
{
    v_->myCookie()->clearCookies();
}

void MainWindow::on_actionScroll_triggered()
{
    //v_->webPage()->scrollMouse(1,1);
    v_->webPage()->startJS("testpos()");
}

void MainWindow::on_actionStatus_triggered()
{
    QPoint p = v_->webPage()->scrollBar();

    qDebug() << p.x() << "," << p.y() ;
}
