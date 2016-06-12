#include <QApplication>
#include "lg_window.h"
#include "lg_login.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    lg_login *login = new lg_login;
    login->exec();

    if (login->get_value()) {

        delete login;
        lg_window *window = new lg_window;
        window->showMaximized();

    } else {

        delete login;
        app.closeAllWindows();
        return 0;
    }

    return app.exec();
}
