#ifndef BAIDU_H
#define BAIDU_H

#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>
#include <QWebHistory>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkProxy>
#include <QNetworkCookie>
#include <QNetworkCookieJar>

#include <QTimer>
#include <QPlainTextEdit>
#include <QCoreApplication>

class baidu_cookie : public QNetworkCookieJar
{

public:
    QList<QNetworkCookie> get_list() {

        return allCookies();
    }
};

class WebBaidu : public QWebView
{

    Q_OBJECT

public:
    WebBaidu();
    void web_start();
    void web_start(QString addr, int port);
    void web_sleep();
    void web_close();

public slots:
    void web_pack(QNetworkReply *reply);
    void web_progress(int progress);
    void web_complete(bool complete);
    void web_open_link(QUrl url);

protected:
    int index_page;//搜索页数
    QNetworkProxy proxy_addr;

public:
    QString link_url;
    QString link_word;
    QString find_word;
    QTextEdit *edit_info;
    QPlainTextEdit *edit_pack;

};

#endif // BAIDU_H
