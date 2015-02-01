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

class WebView;

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
    void sendtojs(QMap<QString, QVariant> object);

public slots:
    //供javascript调用的槽
    QMap<QString, QVariant> slotThatReturns(const QMap<QString, QVariant>& object);
    void slotThatEmitsSignal();
    void scroll(const QMap<QString, QVariant>& object);

private:
    int m_signalEmited;
    QMap<QString, QVariant> m_returnObject;
    QMap<QString, QVariant> m_emitSignal;
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
    WebView(QWidget *parent = 0, MyCookieJar* cookie=NULL);
    WebPage *webPage() const { return m_page; }

    void loadUrl(const QUrl &url);
    QUrl url() const;

    QString lastStatusBarText() const;
    inline int progress() const { return m_progress; }

    void setStatusBarLable(QLabel* label);

    MyCookieJar* myCookie(){return cookieJar_;}

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
    QLabel* status_label_;

    MyCookieJar *cookieJar_;
};

#endif // MYWEBPAGE_H
