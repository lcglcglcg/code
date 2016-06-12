#ifndef KEYWORD_H
#define KEYWORD_H

#include <QUrl>
#include <QKeyEvent>

#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>

class keyword_web : public QWebPage
{

    Q_OBJECT

public:
    keyword_web();
    void keyword_start(QString keyword, int page_max);
    
signals:
    void keyword_signal(QStringList url_list);
    
public slots:
    //void keyword_progress(int progress);
    void keyword_complete(bool complete);

public:
    int page_max;
    int page_index;
    QString keyword;
    QStringList url_list;
};

#endif // KEYWORD_H
