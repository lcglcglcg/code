#include <QTextCodec>
#include <QApplication>
#include "m_demo.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    m_demo *demo = new m_demo;
    demo->showMaximized();

    return app.exec();
}
