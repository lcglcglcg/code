#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "network.h"
#include "task_table.h"
#include "task_insert.h"
#include "task_update.h"
#include "task_delete.h"

#include "chat_window.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(network_tcp *network);
    void create_action();
    void create_menus();
    void create_toolbar();
    void create_status();
    void create_tray();

signals:

public slots:
    void task_insert_run(void);
    void task_update_run(void);
    void task_delete_run(void);

public:
    QAction *action_task_add;
    QAction *action_task_del;
    QAction *action_task_down;
    QAction *action_exit;

    network_tcp *network;
    task_table *task_window;
    chat_widget *chat_window;
    QListWidget *chat_user_list;
};

#endif // MAINWINDOW_H
