#include "baidu.h"

WebBaidu::WebBaidu()
{

    index_page = 0;
    edit_info = new QTextEdit;
    edit_pack = new QPlainTextEdit;
    QWebPage *page = this->page();

    page->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);

    QNetworkAccessManager *manager = page->networkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(web_pack(QNetworkReply*)));

    connect(this, SIGNAL(loadProgress(int)), this, SLOT(web_progress(int)));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(web_complete(bool)));

    //this->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    //connect(this, SIGNAL(linkClicked(QUrl)), this, SLOT(web_open_link(QUrl)));
}

void WebBaidu::web_start()
{
    web_close();
    load(QUrl("http://www.baidu.com"));
}

void WebBaidu::web_start(QString addr, int port)
{

    web_close();

    proxy_addr.setType(QNetworkProxy::HttpProxy);
    proxy_addr.setHostName(addr);
    proxy_addr.setPort(port);
    QNetworkAccessManager *manager = this->page()->networkAccessManager();
    manager->setProxy(proxy_addr);

    QString buffer = "proxy:" + addr;
    edit_info->append(buffer);
    load(QUrl("http://www.baidu.com"));
}


void WebBaidu::web_sleep()
{

    //int sec = rand() % 3;
    QEventLoop eventloop;
    QTimer::singleShot(1, &eventloop, SLOT(quit()));
    eventloop.exec();
}

void WebBaidu::web_pack(QNetworkReply *reply)
{
    QString buffer = "pack:" + reply->url().toEncoded();
    edit_info->append(buffer);
}

void WebBaidu::web_progress(int progress)
{

    QString buffer = "page:";
    buffer += QString::number(index_page);
    buffer += " progress:";
    buffer += QString::number(progress);
    edit_info->append(buffer);
}

void WebBaidu::web_complete(bool complete)
{

    if (!complete) return;
    web_sleep();

    QWebPage *page = this->page();
    QWebFrame  *frame = page->currentFrame();
    if (frame->toPlainText() == "load...") return;
    edit_pack->appendPlainText(frame->toHtml());

    if (index_page == 0) {

        index_page++;
        QWebElement element = frame->findFirstElement("input[id=kw]");
        element.setAttribute("value", find_word);

        QKeyEvent event(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
        this->keyPressEvent(&event);
        return;

    } else {

        index_page++;
        QWebElement document = frame->documentElement();
        QList <QWebElement> element_list = document.findAll("a").toList();

        foreach (QWebElement element, element_list) {

            element.setFocus();

            if (link_word == element.toPlainText()) {

                QString javascript = element.attribute("onmousedown");
                edit_info->append(javascript);
                javascript = javascript.mid(javascript.indexOf(' ') + 1, javascript.size());
                edit_info->append(javascript);

                element.evaluateJavaScript(javascript);
                element.setAttribute("style", "background-color: #ff0000");

                QKeyEvent event(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
                this->keyPressEvent(&event);
                return;

            } else if (element.toPlainText() == "下一页>"){

                QKeyEvent event(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
                this->keyPressEvent(&event);
                return;
            }
        }
    }
}

void WebBaidu::web_open_link(QUrl url)
{

    this->load(url);
}

void WebBaidu::web_close()
{

    for (; index_page; index_page--)
        back();back();back();
    back();back();back();

    this->setHtml(tr("load..."));
    web_sleep();

    edit_pack->clear();
    edit_info->clear();

    QWebPage *page = this->page();
    QWebHistory *history = page->history();
    history->clear();

    for (int n = 0; n < history->count(); n++) {

        QString buffer = "history:" + history->itemAt(n).url().toEncoded();
        edit_info->append(buffer);
    }

    QNetworkAccessManager *manager = page->networkAccessManager();
    QNetworkCookieJar *cookie = new QNetworkCookieJar;
    manager->setCookieJar(cookie);

    baidu_cookie *cookiejar = static_cast<baidu_cookie *>(manager->cookieJar());
    QList<QNetworkCookie> list = cookiejar->get_list();
    foreach (QNetworkCookie coodie, list) {

        QString buffer = "cookie:" + coodie.domain();
        edit_info->append(buffer);
    }

}



