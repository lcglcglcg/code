#ifndef M_THREAD_H
#define M_THREAD_H

#include <QThread>
#include <QApplication>
#include <QDebug>

#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QWebElement>



class m_web : public QWebView
{

    Q_OBJECT

public:
    m_web();

public slots:
    void web_progress(int progress);
    void web_complete(bool complete);

protected:

};

class m_thread : public QThread
{
    Q_OBJECT

public:
    virtual void run();
    
signals:

protected:

};

#endif // M_THREAD_H
