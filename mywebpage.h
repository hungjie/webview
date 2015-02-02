#ifndef MYWEBPAGE_H
#define MYWEBPAGE_H

#include <QWebPage>
#include <QWebView>
#include <QAction>
#include <QMouseEvent>
#include <QLabel>
#include <QMap>
#include <QTimer>

#include <QNetworkCookie>
#include <QNetworkCookieJar>

#include <QScrollBar>

#include <QTabWidget>
#include <QTabBar>

class WebPage;
class WebView;

class MouseOperator
{
public:
    explicit MouseOperator(int x, int y)
    {
        x_ = x;
        y_ = y;
    }

    void LBClick();

    void Move(int x, int y);

    //中键按下
    void MBClick();

    //中键滚动
    void MBRoll(int ch);

private:
    int x_;
    int y_;
};

class TabBar : public QTabBar
{
    Q_OBJECT

signals:
    void closeTab(int index);

public:
    TabBar(QWidget *parent = 0);

private slots:
    void closeTab();

private:
    friend class TabWidget;
};

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);

    WebView *webView(int index) const;
    WebView *currentWebView() const;

public slots:
    WebView *newTab(bool makeCurrent = true);
    void closeTab(int index = -1);

    void loadUrlInCurrentTab(const QUrl &url);

private slots:
    void currentChanged(int index);

private:
    TabBar *m_tabBar;
};

class JsobjectInterface : public QObject
{
    Q_OBJECT
public:
    explicit JsobjectInterface(QObject *parent = 0);

signals:
    void Sendtojs(QMap<QString, QVariant> object);

    void LBClick(QMap<QString, QVariant> object);
    void Move(QMap<QString, QVariant> object);
    void MBClick(QMap<QString, QVariant> object);
    void MBRoll(QMap<QString, QVariant> object);

    void Scroll(const QMap<QString, QVariant>& object);

public slots:
    //供javascript调用的槽
    QMap<QString, QVariant> slotThatReturns(const QMap<QString, QVariant>& object);
    void slotThatEmitsSignal();

    void scroll(const QMap<QString, QVariant>& object);
    void lbclick(const QMap<QString, QVariant>& object);
    void move(const QMap<QString, QVariant>& object);
    void mbclick(QMap<QString, QVariant>& object);
    void mbroll(QMap<QString, QVariant>& object);

private:
    int m_signalEmited;
    QMap<QString, QVariant> m_returnObject;
    QMap<QString, QVariant> m_emitSignal;

    WebPage* page_;
};

class MyCookieJar : public QNetworkCookieJar
{
    Q_OBJECT

public:
    explicit MyCookieJar(QObject *parent = 0);
    ~MyCookieJar();

    QList<QNetworkCookie> mycookies();

    void setCookies(const QList<QNetworkCookie>& cookieList);

    void clearCookies();

    QList<QNetworkCookie> cookieByUrl(QString const& url);

    bool save();
    bool load();
private:

};

class WebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit WebPage(QObject *parent = 0);
    ~WebPage();

    void moveMouse(int x, int y);
    void lefeMouseClicked();
    void scrollMouse(int left, int right);

    void startJS(QString const& func);
    QPoint scrollBar();

signals:
    /*
    void loadLink(const QUrl & url);
    void openLink(const QUrl & url);
    */
    void loadingUrl(const QUrl &url);

protected:
    bool acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type);//重写
    QString userAgentForUrl( const QUrl & url ) const;

private slots:
    void updateMouse();

    void addJavaScriptObject();

private:
    friend class WebView;

    // set the webview mousepressedevent
    Qt::KeyboardModifiers m_keyboardModifiers;
    Qt::MouseButtons m_pressedButtons;
    bool m_openInNewTab;
    QUrl m_loadingUrl;
    QTimer *mouseTimer_;
    int x_;
    int y_;

    QString jQuery;
    QString jscript_;
    JsobjectInterface* jsQObject_;
 };

class WebView : public QWebView {
    Q_OBJECT

public:
    WebView(QWidget *parent = 0);
    ~WebView();

    WebPage *webPage() const { return m_page; }

    void loadUrl(const QUrl &url);
    QUrl url() const;

    QString lastStatusBarText() const;
    inline int progress() const { return m_progress; }

    MyCookieJar* myCookie();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void setProgress(int progress);
    void loadFinished();
    void setStatusBarText(const QString &string);
    void downloadRequested(const QNetworkRequest &request);
    void openLinkInNewTab();
    void linkClicked(const QUrl &);
    void loadStarted();

private:
    QString m_statusBarText;
    QUrl m_initialUrl;
    int m_progress;
    WebPage *m_page;
};

#endif // MYWEBPAGE_H
