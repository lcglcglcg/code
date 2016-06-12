#include "main_window.h"

MainWindow::MainWindow(network_tcp *network)
{

    setContextMenuPolicy (Qt::NoContextMenu);//屏蔽右键菜单

    chat_user_list = new QListWidget;
    QDockWidget *dock_list_user = new QDockWidget(tr("在线用户"));
    dock_list_user->setFeatures(QDockWidget::DockWidgetMovable);
    dock_list_user->setWidget(chat_user_list);
    //addDockWidget(Qt::RightDockWidgetArea, dock_list_user);

    task_window = new task_table;
    chat_window = new chat_widget;
    QTabWidget *edit_tab = new QTabWidget;
    edit_tab->addTab(task_window, tr("任务管理"));
    //edit_tab->addTab(chat_window, tr("对话管理"));
    setCentralWidget(edit_tab);

    this->network = network;
    network->network_set_task_window(task_window);
    network->network_set_chat_window(chat_window);
    network->packet_keyword_download(1);

    create_action();
    create_menus();
    create_toolbar();
    create_status();
    create_tray();


    QStringList str;
    str << "aa" << "bb" << "cc";
    chat_user_list->addItems(str);
}

void MainWindow::create_action()
{

    action_task_add = new QAction(tr("添加任务"), this);
    action_task_add->setIcon(QIcon(tr(":/images/action_task_add.png")));
    connect(action_task_add, SIGNAL(triggered()),
            this, SLOT(task_insert_run()));

    action_task_del = new QAction(tr("修改任务"), this);
    action_task_del->setIcon(QIcon(tr(":/images/action_task_del.png")));
    connect(action_task_del, SIGNAL(triggered()),
            this, SLOT(task_update_run()));

    action_task_down = new QAction(tr("删除任务"), this);
    action_task_down->setIcon(QIcon(tr(":/images/action_task_down.png")));
    connect(action_task_down, SIGNAL(triggered()),
            this, SLOT(task_delete_run()));

    action_exit = new QAction(tr("退出"), this);
    connect(action_exit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

void MainWindow::create_menus()
{

    QMenu *menu = NULL;

    menu = this->menuBar()->addMenu(tr("主菜单"));
    menu->addAction(action_exit);

    menu = this->menuBar()->addMenu(tr("任务管理"));
    menu->addAction(action_task_add);
    menu->addAction(action_task_del);
    menu->addAction(action_task_down);

    menu = this->menuBar()->addMenu(tr("关于"));
    menu->addAction(action_exit);
}

void MainWindow::create_toolbar()
{

    QToolBar *toolbar = new QToolBar(tr("工具栏"));
    toolbar->setIconSize(QSize(50, 50));
    toolbar->addAction(action_task_add);
    toolbar->addAction(action_task_del);
    toolbar->addAction(action_task_down);
    addToolBar(toolbar);
}

void MainWindow::create_status()
{
    statusBar()->showMessage(tr("状态"));
}




void MainWindow::create_tray()
{

}

void MainWindow::task_insert_run()
{

    task_insert task(network);
    task.exec();
}

void MainWindow::task_update_run()
{    

    QStringList keyword = task_window->task_get_keyword();
    task_update task(network, keyword);
    task.exec();
}

void MainWindow::task_delete_run()
{
    QStringList keyword = task_window->task_get_keyword();
    task_delete task(network, keyword);
    task.exec();

    task_window->task_table_del(task.page2_keyword_edit.text());
}
