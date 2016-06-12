#ifndef KEYWORD_H
#define KEYWORD_H

#include <QWebView>
#include <QWebFrame>
#include <QWebElement>
#include <QKeyEvent>
#include <QPlainTextEdit>
#include <QCoreApplication>

#define BAIDU_FIND_STRING "http://www.baidu.com/s?wd="

class KeyWord : public QWebView
{

    Q_OBJECT

public:
    KeyWord(QTextEdit *edit);
    void keyword_start(QString word, int page_max);

signals:
    void signal_keyword_end();

public slots:
    void keyword_progress(int progress);
    void keyword_complete(bool complete);

protected:
    int page_index;
    int page_max;
    QTextEdit *edit;

public:
    QStringList list_word;
    QStringList list_url;
};

#endif // KEYWORD_H
