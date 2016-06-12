#include "m_thread.h"

m_web::m_web()
{
    connect(this, SIGNAL(loadProgress(int)), this, SLOT(web_progress(int)));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(web_complete(bool)));
    load(QUrl("http://www.baidu.com"));
}

void m_web::web_progress(int progress)
{
    qDebug() << "thread:" << qApp->thread()->currentThreadId() << "progress:" << progress;
}

void m_web::web_complete(bool complete)
{

    //qDebug() << "complete = " << complete;
    //QWebFrame  *frame = this->page()->currentFrame();
    //qDebug() << frame->toHtml();

    qDebug() << "thread:" << qApp->thread()->currentThreadId() << "complete:" << complete;
    load(QUrl("http://www.baidu.com"));
}

void m_thread::run()
{

    m_web *web;
    web = new m_web;
    this->exec();
}

