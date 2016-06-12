#include <QTextCodec>
#include <QApplication>
#include "proxy.h"

Proxy_List::Proxy_List()
{

    index = 0;
    connect(this, SIGNAL(loadProgress(int)), this, SLOT(web_progress(int)));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(web_complete(bool)));
    connect(&time, SIGNAL(timeout()), SLOT(web_load()));

    if (!load_proxy_file())
        web_load();
}

int Proxy_List::load_proxy_file()
{

    QFile file(tr("./old_proxy.txt"));
    bool err = file.open(QIODevice::ReadOnly);
    if (!err) {

        file.close();
        qDebug() << "load [old_proxy.txt] error!";
        return -1;
    }

    file.reset();
    while (!file.atEnd()) {

        QString str = file.readLine(0);
        str.chop(2);
        list_str << str;
    }

    file.close();
    return 0;
}

void Proxy_List::web_load()
{
    QString addr = list_str.at(index);
    QString port = addr.section(':', 1 ,1);
    addr.chop(port.size() + 1);

    QString url = "http://www.baidu.com/s?wd=" + addr;
    currentFrame()->load(QUrl(url));
}

void Proxy_List::web_progress(int progress)
{
    qDebug() << "progress:" << progress << "%";
}

void Proxy_List::web_complete(bool complete)
{

    if (!complete) return;
    time.stop();

    QWebFrame  *frame = this->currentFrame();
    QString str = frame->toHtml();

    str = str.mid(str.indexOf("来&nbsp;&nbsp;&nbsp;自:&nbsp;"), 128);
    int start = str.indexOf("<strong>") + strlen("<strong>");
    int end = str.indexOf("</strong>");
    str = str.mid(start, end - start);

    QString str_temp = list_str.at(index) + "[" + str + "]";

    list_new << str_temp;
    qDebug() << index << list_str.at(index) << "[OK]";

    if (++index >= list_str.size()) {

        QFile file(tr("./new_proxy.txt"));
        file.open(QIODevice::WriteOnly);

        qSort(list_new.begin(), list_new.end());

        foreach (QString item, list_new) {

            file.write(item.toLocal8Bit());
            file.write("\r\n");
        }

        qDebug() << "all Complete!";
        file.close();
        return;
    }

    web_load();
    time.start(3000);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    Proxy_List *proxy = NULL;
    proxy = new Proxy_List;

    return a.exec();
}
