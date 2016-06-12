#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui>

class Dialog : public QDialog
{

    Q_OBJECT

public:

    Dialog();
    ~Dialog();

    QVBoxLayout         button_layout;
    QPushButton         button_normal;
    QPushButton         button_failure;
    QPushButton         button_message;

    QSystemTrayIcon     tray_icon;
    QIcon               tray_icon_normal;
    QIcon               tray_icon_failure;
    QMenu               tray_icon_menu;
    QAction             *tray_icon_menu_normal;
    QAction             *tray_icon_menu_failure;
    QAction             *tray_icon_menu_quit;



public slots:
    void tray_icon_init();
    void tray_icon_set_normal();
    void tray_icon_set_failure();
    void tray_icon_message();
};

#endif // DIALOG_H
