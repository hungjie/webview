#include "mywebpage.h"

#include <QApplication>
#include <QClipboard>
#include <QNetworkRequest>

#include <QNetworkCookie>

#include <QWebFrame>
#include <QCursor>

#include "mainwindow.h"

JsobjectInterface::JsobjectInterface(QObject *parent)
    : QObject(parent)
{
    m_signalEmited = 0;
}

QMap<QString, QVariant> JsobjectInterface::slotThatReturns(const QMap<QString, QVariant>& object)
{
    qDebug() << "SampleQObject::slotThatReturns";
    this->m_returnObject.clear();
    this->m_returnObject.unite(object);
    QString addedBonus = QString::number(object["intValue"].toInt(),
                                         10).append(" added bonus.");
    this->m_returnObject["stringValue"] = QVariant(addedBonus);
    qDebug() << "SampleQObject::slotThatReturns" << this->m_returnObject;

    return this->m_returnObject;
}

void JsobjectInterface::slotThatEmitsSignal()
{
    qDebug() << "SampleQObject::slotThatEmitsSignal";
    this->m_signalEmited++;
    this->m_emitSignal.clear();
    this->m_emitSignal["signalsEmited"] = QVariant(this->m_signalEmited);
    this->m_emitSignal["sender"] = QVariant("SampleQObject::slotThatEmitsSignal");
    qDebug() << "SampleQObject::slotThatEmitsSignal" << this->m_emitSignal;
    emit sendtojs(this->m_emitSignal);
}

void JsobjectInterface::scroll(const QMap<QString, QVariant> &object)
{

}

MyCookieJar::MyCookieJar(QObject *parent)
    : QNetworkCookieJar(parent)
{

}

MyCookieJar::~MyCookieJar()
{
}

QList<QNetworkCookie> MyCookieJar::mycookies()
{
    return this->allCookies();
}

void MyCookieJar::setCookies(const QList<QNetworkCookie>& cookieList)
{
    this->setAllCookies(cookieList);
}

void MyCookieJar::clearCookies()
{
    QList<QNetworkCookie> cookieList;
    this->setAllCookies(cookieList);
}

QList<QNetworkCookie> MyCookieJar::cookieByUrl(const QString &url)
{
    return this->cookiesForUrl(QUrl(url));
}

bool MyCookieJar::save()
{
    QFile file;
    file.setFileName("cookie.dat");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QList<QNetworkCookie> list = this->allCookies();
    QByteArray str;
    QNetworkCookie cookie;
    foreach (cookie, list)
    {
        str += cookie.toRawForm() + "\n";
    }

    file.write(str);
    //QTextStream stream(&file);
    //stream << str;
    file.close();

    return true;
}

bool MyCookieJar::load()
{
    QFile file;
    file.setFileName("cookie.dat");
    file.open(QIODevice::ReadOnly);
    QString str = file.readAll();
    file.close();

    if(!str.isEmpty())
    {
        this->setAllCookies(QNetworkCookie::parseCookies(str.toUtf8()));
        return true;
    }

    return false;
}

WebPage::WebPage(QObject *parent)
    : QWebPage(parent)
    , m_keyboardModifiers(Qt::NoModifier)
    , m_pressedButtons(Qt::NoButton)
    , m_openInNewTab(false)
    , x_(0)
    , y_(0)
{
    //setNetworkAccessManager(BrowserApplication::networkAccessManager());
    //connect(this, SIGNAL(unsupportedContent(QNetworkReply*)),
    //        this, SLOT(handleUnsupportedContent(QNetworkReply*)));

    QFile file;
    file.setFileName("jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    jQuery.append("\nvar qt = { 'jQuery': jQuery.noConflict(true) };");
    file.close();

    QFile file2;
    file2.setFileName("test.js");
    file2.open(QIODevice::ReadOnly);
    jscript_ = file2.readAll();
    file2.close();

    mouseTimer_ = new QTimer(this);
    connect(mouseTimer_, SIGNAL(timeout()), this, SLOT(updateMouse()));

    jsQObject_ = new JsobjectInterface(this);

    connect(mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(addJavaScriptObject()));
}

void WebPage::moveMouse(int x, int y)
{
    x_ = x;
    y_ = y;
    mouseTimer_->start(10);
}

void WebPage::lefeMouseClicked()
{
    QMouseEvent *mEvnPress;
    QMouseEvent *mEvnRelease;
    mEvnPress = new QMouseEvent(QEvent::MouseButtonPress, QCursor::pos(), Qt::LeftButton, Qt::LeftButton,Qt::NoModifier);
    QApplication::sendEvent(this, mEvnPress);
    mEvnRelease = new QMouseEvent(QEvent::MouseButtonRelease, QCursor::pos(), Qt::LeftButton, Qt::LeftButton,Qt::NoModifier);
    QApplication::sendEvent(this, mEvnRelease);
}

void WebPage::scrollMouse(int left, int right)
{
    mainFrame()->scroll(left, right);
}

void WebPage::startJS(const QString &func)
{
    mainFrame()->evaluateJavaScript(func);
}

void WebPage::updateMouse()
{
    QPoint p = QCursor::pos();
    int x = p.x();
    int y = p.y();

    if(x > x_)
    {
        x = x - 1;
    }
    else if( x < x_)
    {
        x = x + 1;
    }

    if(y > y_)
    {
        y = y -1;
    }
    else if(y < y_)
    {
        y = y + 1;
    }

    QCursor::setPos(x, y);

    if(x == x_ && y == y_)
    {
        mouseTimer_->stop();
    }
}

void WebPage::addJavaScriptObject()
{
    mainFrame()->evaluateJavaScript(jQuery);

    mainFrame()->addToJavaScriptWindowObject("jsQObject", jsQObject_);

    mainFrame()->evaluateJavaScript(jscript_);
    //mainFrame()->evaluateJavaScript(QString("func()"));
}

bool WebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type)
{
    /*
    if(type==0){//如果是用户点击
        if(frame!=mainFrame()){ //如果不是在本窗口的连接
            emit openLink(request.url());//发送打开新连接信号
        }else{
            emit loadLink(request.url());//发送点击连接信号
        }

    }
    */
    return QWebPage::acceptNavigationRequest(frame, request, type);
}

QString WebPage::userAgentForUrl(const QUrl &url) const
{
    //return "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.2 (KHTML, like Gecko) Chrome/15.0.874.121 Safari/535.2";
    return "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:8.0) Gecko/20100101 Firefox/8.0";
}

QPoint WebPage::scrollBar()
{
    return mainFrame()->scrollPosition();
}

WebView::WebView(QWidget* parent, MyCookieJar* cookieJar)
    : QWebView(parent)
    , m_progress(0)
    , m_page(new WebPage(this))
    , cookieJar_(cookieJar)
{
    setPage(m_page);

    connect(page(), SIGNAL(statusBarMessage(QString)),
            SLOT(setStatusBarText(QString)));
    connect(this, SIGNAL(loadProgress(int)),
            this, SLOT(setProgress(int)));
    connect(this, SIGNAL(loadStarted()),
            this, SLOT(loadStarted()));
    connect(this, SIGNAL(loadFinished(bool)),
            this, SLOT(loadFinished()));
    connect(page(), SIGNAL(loadingUrl(QUrl)),
            this, SIGNAL(urlChanged(QUrl)));
    connect(page(), SIGNAL(downloadRequested(QNetworkRequest)),
            this, SLOT(downloadRequested(QNetworkRequest)));

    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebSettings::PluginsEnabled, true);

    page()->setForwardUnsupportedContent(true);
    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(this, SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)));

    if(cookieJar_ == NULL)
    {
        cookieJar_ = new MyCookieJar();
        qDebug() << cookieJar_->load();
    }
    page()->networkAccessManager()->setCookieJar( cookieJar_ );
}

void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    /*
    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());
    if (!r.linkUrl().isEmpty()) {
        QMenu menu(this);
        menu.addAction(pageAction(QWebPage::OpenLinkInNewWindow));
        menu.addAction(tr("Open in New Tab"), this, SLOT(openLinkInNewTab()));
        menu.addSeparator();
        menu.addAction(pageAction(QWebPage::DownloadLinkToDisk));
        // Add link to bookmarks...
        menu.addSeparator();
        menu.addAction(pageAction(QWebPage::CopyLinkToClipboard));
        if (page()->settings()->testAttribute(QWebSettings::DeveloperExtrasEnabled))
            menu.addAction(pageAction(QWebPage::InspectElement));
        menu.exec(mapToGlobal(event->pos()));
        return;
    }
    */
    QWebView::contextMenuEvent(event);
}

void WebView::wheelEvent(QWheelEvent *event)
{
    /*
    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;
        setTextSizeMultiplier(textSizeMultiplier() + numSteps * 0.1);
        event->accept();
        return;
    }
    */
    QWebView::wheelEvent(event);
}

void WebView::openLinkInNewTab()
{
    m_page->m_openInNewTab = true;
    pageAction(QWebPage::OpenLinkInNewWindow)->trigger();
}

void WebView::setProgress(int progress)
{
    m_progress = progress;
}

void WebView::loadStarted()
{
    m_progress = 0;
    //webPage()->myCookie()->clearCookies();
}

void WebView::loadFinished()
{
    if (100 != m_progress) {
        qWarning() << "Received finished signal while progress is still:" << progress()
                   << "Url:" << url();
    }
}

void WebView::loadUrl(const QUrl &url)
{
    m_initialUrl = url;

    cookieJar_->setCookiesFromUrl(cookieJar_->mycookies(), url);
    load(url);
}

QString WebView::lastStatusBarText() const
{
    return m_statusBarText;
}

QUrl WebView::url() const
{
    QUrl url = QWebView::url();
    if (!url.isEmpty())
        return url;

    return m_initialUrl;
}

void WebView::mousePressEvent(QMouseEvent *event)
{
    m_page->m_pressedButtons = event->buttons();
    m_page->m_keyboardModifiers = event->modifiers();
    QWebView::mousePressEvent(event);
}

void WebView::mouseReleaseEvent(QMouseEvent *event)
{
    QWebView::mouseReleaseEvent(event);
    if (!event->isAccepted() && (m_page->m_pressedButtons & Qt::MidButton)) {
        QUrl url(QApplication::clipboard()->text(QClipboard::Selection));
        if (!url.isEmpty() && url.isValid() && !url.scheme().isEmpty()) {
            setUrl(url);
        }
    }
}

void WebView::setStatusBarLable(QLabel* label)
{
    status_label_ = label;
}

void WebView::mouseMoveEvent(QMouseEvent *event)
{
    //parent_->mouseMoveEvent(event);
    status_label_->setText("("+QString::number(event->x())+","+QString::number(event->y())+")");
}

void WebView::setStatusBarText(const QString &string)
{
    m_statusBarText = string;
}

void WebView::downloadRequested(const QNetworkRequest &request)
{
    //BrowserApplication::downloadManager()->download(request);
}

void WebView::linkClicked(QUrl const& url)
{
    loadUrl(url);
    //ui->lineEdit->setText(url.toString());
}
