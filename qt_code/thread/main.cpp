#include <QTextCodec>
#include <QApplication>
#include "m_thread.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    m_thread *a = new m_thread;
    a->start();

    m_thread *b = new m_thread;
    b->start();

    m_thread *c = new m_thread;
    c->start();

    m_thread *d = new m_thread;
    d->start();

    m_thread *e = new m_thread;
    e->start();

    //for (int n = 0; n < 10; n++) th[n].start();
    //for (int n = 0; n < 10; n++) th[n].wait();

    return app.exec();
}
