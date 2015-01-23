#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    v_ = new WebView(this);
    ui->verticalLayout->addWidget(v_);

    v_->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(v_, SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::linkClicked(QUrl const& url)
{
    v_->loadUrl(url);
    ui->lineEdit->setText(url.toString());
}

void MainWindow::on_pushButton_clicked()
{
    if(!ui->lineEdit->text().isEmpty())
    {
        QUrl url(ui->lineEdit->text());
        v_->loadUrl(url);
    }
}
