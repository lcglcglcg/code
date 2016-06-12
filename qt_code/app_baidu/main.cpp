#include <QApplication>
#include <QTextCodec>

#include "network.h"
#include "login_window.h"
#include "main_window.h"

#include "task_insert.h"
#include "task_update.h"
#include "task_delete.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));

    network_tcp *network = new network_tcp;

#if 10
    login_window *login = new login_window(network);
    login->exec();
    if (login->get_value() == -1) {

        login->deleteLater();
        return -1;
    }

    MainWindow *window = new MainWindow(network);
    window->showMaximized();
#endif


    //task_insert task(network);
    //task_update task(network);
    //task_delete task(network);
    //task.exec();

    return app.exec();
}
