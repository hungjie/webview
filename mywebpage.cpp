#include "mywebpage.h"

#include <QApplication>
#include <QClipboard>
#include <QNetworkRequest>

#include <QNetworkCookie>

#include <QWebFrame>
#include <QCursor>
#include <QPalette>

#include <QThread>

#include <QErrorMessage>

#include <QWebElement>

#include "mainwindow.h"

//#include <QtWebEngineWidgets/QWebEnginePage>

#define NOMINMAX
#include "windows.h"

int JsobjectInterface::m_signalEmited = 0;

void MouseOperator::LBClick()
{
    ::mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_LEFTUP, x_, y_, 0, 0);
}

void MouseOperator::Move(int x, int y)
{
    x_ = x;
    y_ = y;

    ::SetCursorPos(x, y);
}

//中键按下
void MouseOperator::MBClick()
{
    ::mouse_event(MOUSEEVENTF_MIDDLEDOWN | MOUSEEVENTF_MIDDLEUP, x_, y_, 0, 0);
}

//中键滚动
void MouseOperator::MBRoll(int ch)
{
    ::mouse_event(MOUSEEVENTF_WHEEL, x_, y_, ch, 0);
}

JsobjectInterface::JsobjectInterface(QObject *parent)
    : QObject(0)
    , page_(parent)
{
    mouseMoveTimer_ = new QTimer(this);
    connect(mouseMoveTimer_, SIGNAL(timeout()), this, SLOT(updateMouseMove()));

    mouseScrollTimer_ = new QTimer(this);
    connect(mouseScrollTimer_, SIGNAL(timeout()), this, SLOT(updateMouseScroll()));

    inputTimer_ = new QTimer(this);
    connect(inputTimer_, SIGNAL(timeout()), this, SLOT(updateTimerInput()));

    sleepTimer_ = new QTimer(this);
    connect(sleepTimer_, SIGNAL(timeout()), this, SLOT(updateSleep()));
}

QMap<QString, QVariant> JsobjectInterface::slotThatReturns(const QMap<QString, QVariant>& object)
{
    qDebug() << "SampleQObject::slotThatReturns";
    this->m_returnObject.clear();
    this->m_returnObject.unite(object);
    QString addedBonus = QString::number(object["intValue"].toInt(), 10).append(" added bonus.");
    this->m_returnObject["stringValue"] = QVariant(addedBonus);
    qDebug() << "SampleQObject::slotThatReturns" << this->m_returnObject;

    return this->m_returnObject;
}

void JsobjectInterface::sleep(QVariant const &object)
{
    int time = object.toInt();
    //QThread::msleep(time);

    sleepTimer_->start(time);
}

QVariant JsobjectInterface::get_search_input_array()
{
    QStringList list;
    list << QString("L") << QString::fromLocal8Bit("o");

    return QVariant::fromValue(list);
}

QVariant JsobjectInterface::get_search_input_id()
{
    return QVariant::fromValue(QString("kw"));
}

void JsobjectInterface::timerInput(const QMap<QString, QVariant> &object)
{
    m_emitSignal = object;

    /*
    int cur_index = object["cur_index"].toInt();
    QStringList sl = object["input_array"].toStringList();

    if(cur_index < sl.length())
    {
        --this->m_signalEmited;
    }
    */

    int time = m_emitSignal["time"].toInt();

    inputTimer_->start(time);
}

void JsobjectInterface::scroll(const QMap<QString, QVariant> &object)
{
    qDebug()<<"test";
    int left = object["left"].toInt();
    int top = object["top"].toInt();

//    QErrorMessage m(0);
//    m.showMessage(QString::number(left) + "," + QString::number(top));
//    m.exec();
    scroll_x_ = left;
    scroll_y_ = top;

    mouseScrollTimer_->start(20);

    qDebug() << scroll_x_ << "," << scroll_y_;

    //qobject_cast<WebPage*>(page_)->scrollBar(scroll_x_, scroll_y_);
}

void JsobjectInterface::lbclick(const QMap<QString, QVariant> &object)
{
    int x = object["left"].toInt();
    int y = object["top"].toInt();

    if(x != -1 && y != -1)
        nativeToGlobal(x, y);
    else
    {
        x = QCursor::pos().x();
        y = QCursor::pos().y();
    }

    MouseOperator op(x, y);
    op.LBClick();

    QMap<QString, QVariant> emitSignal;
//    emitSignal["top"] = QVariant(y);
//    emitSignal["left"] = QVariant(x);

    emitToJs("lbclick", emitSignal);
}

void JsobjectInterface::move(const QMap<QString, QVariant> &object)
{
    move_x_ = object["left"].toInt();
    move_y_ = object["top"].toInt();

    nativeToGlobal(move_x_, move_y_);

    mouseMoveTimer_->start(20);
}

void JsobjectInterface::mbclick(QMap<QString, QVariant> &object)
{
    int x = object["left"].toInt();
    int y = object["top"].toInt();

    if(x != -1 && y != -1)
        nativeToGlobal(x, y);
    else
    {
        x = QCursor::pos().x();
        y = QCursor::pos().y();
    }

    MouseOperator op(x, y);

    op.MBClick();

    QMap<QString, QVariant> emitSignal;
    //emitSignal["top"] = QVariant(y);
    //emitSignal["left"] = QVariant(x);

    emitToJs("mbclick", emitSignal);
}

void JsobjectInterface::mbroll(QMap<QString, QVariant> &object)
{
    int x = object["left"].toInt();
    int y = object["top"].toInt();

    int ch = object["ch"].toInt();

    if(x != -1 && y != -1)
        nativeToGlobal(x, y);
    else
    {
        x = QCursor::pos().x();
        y = QCursor::pos().y();
    }

    MouseOperator op(x, y);

    op.MBRoll(ch);

    QMap<QString, QVariant> emitSignal;
    //emitSignal["top"] = QVariant(y);
    //emitSignal["left"] = QVariant(x);

    emitToJs("mbroll", emitSignal);
}

QVariant JsobjectInterface::isLoadFinished()
{
    bool res = qobject_cast<WebPage*>(page_)->isLoadFinished();
    return QVariant::fromValue(res);
}

void JsobjectInterface::updateMouseMove()
{
    QPoint p = QCursor::pos();
    int x = p.x();
    int y = p.y();

    if(x > move_x_)
    {
        x = x - 1;
    }
    else if( x < move_x_)
    {
        x = x + 1;
    }

    if(y > move_y_)
    {
        y = y -1;
    }
    else if(y < move_y_)
    {
        y = y + 1;
    }

    QCursor::setPos(x, y);

    if(x == move_x_ && y == move_y_)
    {
        mouseMoveTimer_->stop();

        QMap<QString, QVariant> emitSignal;
//        emitSignal["top"] = QVariant(move_y_);
//        emitSignal["left"] = QVariant(move_x_);

        emitToJs("updateMouseMove", emitSignal);
    }
}

void JsobjectInterface::updateMouseScroll()
{
    int left = 0;
    int top = 0;

    WebPage* page = qobject_cast<WebPage*>(page_);
    QPoint curBar = page->scrollBar();
    left = scroll_x_ - curBar.x();
    top = scroll_y_ - curBar.y();

    int scroll_vertical_max = page->maxVerticalScrollBar();
    int scroll_horizontal_max = page->maxHorizontalScrollBar();

    if( scroll_vertical_max == 0 )
    {
        top = 0;
    }

    if( scroll_horizontal_max == 0 )
    {
        left = 0;
    }

    int left_action = 0;
    int top_action = 0;
    if(top > 0 && scroll_vertical_max != curBar.y())
    {
        top_action++;
    }
    else if(top < 0 && 0 != curBar.y())
    {
        top_action--;
    }
    else
    {
        top = 0;
    }

    if(top == 0)
    {
        if(left > 0 && scroll_horizontal_max != curBar.x())
        {
            left_action++;
        }
        else if(left < 0 && scroll_horizontal_max != 0)
        {
            left_action--;
        }
        else
        {
            mouseScrollTimer_->stop();

            QMap<QString, QVariant> emitSignal;
//            emitSignal["scroll_y"] = QVariant(scroll_y_);
//            emitSignal["scroll_x"] = QVariant(scroll_x_);
/*
            QStringList list;
            list << QString("first") << QString("second");

            emitSignal["add"] = QVariant::fromValue(list);
*/
            emitToJs("updateMouseScroll", emitSignal);
        }
    }

    qobject_cast<WebPage*>(page_)->scrollBar(left_action, top_action);
}

void JsobjectInterface::updateTimerInput()
{
    int cur_index = m_emitSignal["cur_index"].toInt();
    QStringList sl = m_emitSignal["input_array"].toStringList();
    QString id = m_emitSignal["input_id"].toString();

    QString find = QString("input[id=%1]").arg(id);
    QWebElement e = qobject_cast<WebPage*>(page_)->mainFrame()->findFirstElement(find);

    QString input = m_emitSignal["cur_input"].toString();

    if(cur_index < sl.length())
    {
        input += sl[cur_index];
        m_emitSignal["cur_input"] = QVariant::fromValue(input);

        e.setAttribute("value", input);

        cur_index++;
        m_emitSignal["cur_index"] = QVariant::fromValue(cur_index);

        //qDebug() << input;

        return;
    }

    emitToJs("updateTimerInput", m_emitSignal);
}

void JsobjectInterface::updateSleep()
{
    emitToJs("updateSleep", m_emitSignal);
}

void JsobjectInterface::emitToJs(QString const& sender, const QMap<QString, QVariant> &object)
{
    this->m_emitSignal.clear();
    this->m_emitSignal = object;

    this->m_emitSignal["sender"] = QVariant(sender);
    this->m_emitSignal["signalsEmited"] = ++this->m_signalEmited;

    emit Sendtojs(this->m_emitSignal);
}

void JsobjectInterface::nativeToGlobal(int &x, int &y)
{
    QPoint curBar = qobject_cast<WebPage*>(page_)->scrollBar();

    x -= curBar.x();
    y -= curBar.y();

    QPoint vp = MainWindow::Instance()->viewPos();
    x += vp.x();
    y += vp.y();
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
    , is_load_finished_(false)
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

    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));

    jsQObject_ = new JsobjectInterface(this);

    connect(mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(addJavaScriptObject()));
}

WebPage::~WebPage()
{
    //delete jsQObject_;
}

void WebPage::lefeMouseClicked()
{

}

void WebPage::scrollBar(int left, int right)
{
    mainFrame()->scroll(left, right);
}

void WebPage::startJS(const QString &func)
{
    mainFrame()->evaluateJavaScript(func);
}

void WebPage::addJavaScriptObject()
{
    //mainFrame()->evaluateJavaScript(jQuery);

    mainFrame()->addToJavaScriptWindowObject("jsQObject", jsQObject_);

    //mainFrame()->evaluateJavaScript(QString("var globalvar = 'hello';"));
    mainFrame()->evaluateJavaScript(jscript_);
    //mainFrame()->evaluateJavaScript(QString("func()"));
}

void WebPage::loadFinished(bool ok)
{
    is_load_finished_ = ok;
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
    if (type == 0 && m_pressedButtons == Qt::MidButton) {
        WebView *webView;
        //bool selectNewTab = (m_keyboardModifiers & Qt::ShiftModifier);
        webView = MainWindow::Instance()->tabWidget()->newTab(false);

        webView->setFocus();

        webView->load(request);
        m_keyboardModifiers = Qt::NoModifier;
        m_pressedButtons = Qt::NoButton;
        return false;
    }

    m_loadingUrl = request.url();
    emit loadingUrl(m_loadingUrl);

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

int WebPage::maxScrollBar(Qt::Orientation orientation)
{
    return mainFrame()->scrollBarMaximum(orientation);
}

int WebPage::maxVerticalScrollBar()
{
    return maxScrollBar(Qt::Vertical);
}

int WebPage::maxHorizontalScrollBar()
{
    return maxScrollBar(Qt::Horizontal);
}

WebView::WebView(QWidget* parent)
    : QWebView(parent)
    , m_progress(0)
    , m_page(new WebPage(0))
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

    /*
    if(cookieJar_ == NULL)
    {
        cookieJar_ = new MyCookieJar();
        qDebug() << cookieJar_->load();
    }
    */
    page()->networkAccessManager()->setCookieJar( MainWindow::Instance()->myCookie() );
}

WebView::~WebView()
{
    //delete m_page;
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

    myCookie()->setCookiesFromUrl(myCookie()->mycookies(), url);
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

MyCookieJar *WebView::myCookie()
{
    return MainWindow::Instance()->myCookie();
}

void WebView::mouseMoveEvent(QMouseEvent *event)
{
    //parent_->mouseMoveEvent(event);
    MainWindow::Instance()->statusLabel()->setText("("+QString::number(event->x())+","+QString::number(event->y())+")");
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


TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    setTabsClosable(true);
    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SIGNAL(closeTab(int)));
    setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
}

void TabBar::closeTab()
{
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        int index = action->data().toInt();
        emit closeTab(index);
    }
}

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
    , m_tabBar(new TabBar(this))
{
    connect(m_tabBar, SIGNAL(closeTab(int)), this, SLOT(closeTab(int)));

    setTabBar(m_tabBar);

    setDocumentMode(true);

    newTab(true);
}

WebView *TabWidget::newTab(bool makeCurrent)
{
    //*
    if (count() == 0) {
        QWidget *emptyWidget = new QWidget;
        QPalette p = emptyWidget->palette();
        p.setColor(QPalette::Window, palette().color(QPalette::Base));
        emptyWidget->setPalette(p);
        emptyWidget->setAutoFillBackground(true);
        disconnect(this, SIGNAL(currentChanged(int)),this, SLOT(currentChanged(int)));
        addTab(emptyWidget, tr("(Untitled)"));
        connect(this, SIGNAL(currentChanged(int)),this, SLOT(currentChanged(int)));
        return 0;
    }
    //*/

    // webview
    WebView *webView = new WebView;

    webView->setMouseTracking(true);

    addTab(webView, tr("(Untitled)"));
    if (makeCurrent)
        setCurrentWidget(webView);

    if (count() == 1)
        currentChanged(currentIndex());
    //emit tabsChanged();
    return webView;
}

void TabWidget::closeTab(int index)
{
    if (index < 0)
        index = currentIndex();
    if (index < 0 || index >= count())
        return;

    removeTab(index);

    /*
    QWidget *webView = widget(index);
    if(webView)
        delete webView;
    //*/
/*
    if(count() == 0)
    {
        newTab(true);
    }
*/
}

void TabWidget::currentChanged(int index)
{
    WebView *webView = this->webView(index);
    if (!webView)
        return;

    webView->setFocus();
}

WebView *TabWidget::webView(int index) const
{
    QWidget *widget = this->widget(index);
    if (WebView *webView = qobject_cast<WebView*>(widget)) {
        return webView;
    } else {
        // optimization to delay creating the first webview
        if (count() == 1) {
            TabWidget *that = const_cast<TabWidget*>(this);
            that->setUpdatesEnabled(false);
            that->newTab();
            that->closeTab(0);
            that->setUpdatesEnabled(true);
            return currentWebView();
        }
    }

    return 0;
}

WebView *TabWidget::currentWebView() const
{
    return webView(currentIndex());
}

void TabWidget::loadUrlInCurrentTab(const QUrl &url)
{
    WebView *webView = currentWebView();
    if (webView) {
        webView->loadUrl(url);
        webView->setFocus();
    }
}
