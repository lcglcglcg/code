#ifndef PROXY_H
#define PROXY_H

#include <QTimer>
#include <QWebPage>
#include <QWebFrame>

#include <QFile>
#include <QDebug>
#include <QMessageBox>


class Proxy_List : public QWebPage
{
    Q_OBJECT

public:
    Proxy_List();
    int load_proxy_file();

signals:

public slots:
    void web_progress(int progress);
    void web_complete(bool complete);
    void web_load();

public:
    int index;
    QTimer time;
    QStringList list_str;
    QStringList list_new;

};

#endif // PROXY_H

