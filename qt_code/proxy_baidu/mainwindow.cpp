#include "mainwindow.h"

MainWindow::MainWindow()
{

    create_action();
    //create_menus();
    create_toolbar();
    create_window();
    //create_status();
    create_tray();

    load_proxy_file();
    load_config_file();
}

int MainWindow::load_proxy_file()
{

    QFile file(tr("./http_proxy.ini"));
    bool err = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!err) {

        QMessageBox mess;
        mess.setText("load http_proxy.ini error!");
        mess.exec();
        return -1;
    }

    file.reset();
    while (!file.atEnd()) {

        QString str = file.readLine(0);
        str.chop(1);
        qDebug() << str;
        proxy_str_list << str;
    }

    proxy_list.addItems(proxy_str_list);
    file.close();
    return 0;
}

int MainWindow::load_config_file()
{

    int link_url_size = 0;
    int link_keyword_size = 0;
    QFile file(tr("./config.ini"));
    bool err = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!err) {

        QMessageBox mess;
        mess.setText("load config.ini error!");
        mess.exec();
        return -1;
    }

    file.reset();
    while (!file.atEnd()) {

        QString options = file.readLine(0);
        options.chop(1);
        QString value = options.mid(options.indexOf('=') + 1, options.size());
        options.chop(value.size() + 1);

        if (options == "find_keyword") {

            toolbar_edit_find->setText(value);

        } else if (options == "page_depth") {

            toolbar_edit_depth->setText(value);

        } else if (options == "time_proxy") {

            toolbar_edit_time_proxy->setText(value);

        } else if (options == "time_wait") {

            toolbar_edit_time_wait->setText(value);

        } else if (options == "link_keyword_str") {

            toolbar2_edit_word->setText(value);

        } else if (options == "link_keyword_size") {

            link_keyword_size = value.toInt();

        } else if (options == "link_url_str") {

            toolbar2_edit_url->setText(value);

        } else if (options == "link_url_size") {

            link_url_size = value.toInt();
        }
    }

    file.close();
    if (link_keyword_size == link_url_size)
        return link_keyword_size;
    return -1;
}

int MainWindow::save_config_file()
{
    QFile file(tr("./config.ini"));
    bool err = file.open(QIODevice::WriteOnly);
    if (!err) {

        QMessageBox mess;
        mess.setText("save config.ini error!");
        mess.exec();
        return -1;
    }

    file.write("find_keyword=");
    file.write(toolbar_edit_find->text().toLocal8Bit());
    file.write("\r\n");

    file.write("page_depth=");
    file.write(toolbar_edit_depth->text().toLocal8Bit());
    file.write("\r\n");

    file.write("time_proxy=");
    file.write(toolbar_edit_time_proxy->text().toLocal8Bit());
    file.write("\r\n");

    file.write("time_wait=");
    file.write(toolbar_edit_time_wait->text().toLocal8Bit());
    file.write("\r\n");

    file.write("link_keyword_str=");
    file.write(toolbar2_edit_word->text().toLocal8Bit());
    file.write("\r\n");

    file.write("link_keyword_size=");
    file.write(QString::number(keyword->list_word.size()).toLocal8Bit());
    file.write("\r\n");

    file.write("link_url_str=");
    file.write(toolbar2_edit_url->text().toLocal8Bit());
    file.write("\r\n");

    file.write("link_url_size=");
    file.write(QString::number(keyword->list_url.size()).toLocal8Bit());
    file.write("\r\n");

    file.close();
    return 0;
}

int MainWindow::load_keyword_file(int size)
{
    QFile file(tr("./keyword.ini"));
    bool err = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!err) {

        QMessageBox mess;
        mess.setText("load keyword.ini error!");
        mess.exec();
        return -1;
    }

    file.reset();
    int record = 0;
    QString buffer = file.readAll();

    for (int n = 0; n < buffer.size(); n++) {

        if (buffer.at(n) == '\n' && buffer.at(n - 1) == '\n') {

            QString str = buffer.mid(record, n - record - 1);
            record = ++n;

            if (size-- > 0) keyword->list_word << str;
            else keyword->list_url << str;
        }
    }

    toolbar2_completer_word = new QCompleter(keyword->list_word);
    toolbar2_completer_url = new QCompleter(keyword->list_url);
    toolbar2_edit_word->setCompleter(toolbar2_completer_word);
    toolbar2_edit_url->setCompleter(toolbar2_completer_url);

    file.close();
    return 0;
}

int MainWindow::save_keyword_file()
{

    QFile file(tr("./keyword.ini"));
    bool err = file.open(QIODevice::WriteOnly);
    if (!err) {

        QMessageBox mess;
        mess.setText("save keyword.ini error!");
        mess.exec();
        return -1;
    }

    foreach (QString str, keyword->list_word) {

        file.write(str.toLocal8Bit());
        file.write("\r\n\r\n");
    }

    foreach (QString str, keyword->list_url) {

        file.write(str.toLocal8Bit());
        file.write("\r\n\r\n");
    }

    file.close();
    return 0;
}

void MainWindow::create_action()
{

    action_find = new QAction(tr(ACTION_FIND), this);
    connect(action_find, SIGNAL(triggered()), this, SLOT(action_find_run()));

    action_start = new QAction(tr(ACTION_START_START), this);
    action_start->setCheckable(true);
    connect(action_start, SIGNAL(triggered(bool)), this, SLOT(action_start_run(bool)));

    action_show = new QAction(tr(ACTION_SHOW), this);
    connect(action_show, SIGNAL(triggered()), this, SLOT(show()));

    action_hide = new QAction(tr(ACTION_HIDE), this);
    connect(action_hide, SIGNAL(triggered()), this, SLOT(hide()));

    action_exit = new QAction(tr(ACTION_EXIT), this);
    connect(action_exit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    //快捷键
    //action_start->setShortcuts(QKeySequence::Open);
    //action_exit->setShortcuts(QKeySequence::Close);
    //action_clear->setShortcuts(QKeySequence::DeleteStartOfWord);
    //action_web->setShortcuts(QKeySequence::DeleteEndOfWord);
}

void MainWindow::create_menus()
{

    QMenu *menu_system = menuBar()->addMenu(tr("menu"));
    menu_system->addAction(action_find);
    menu_system->addAction(action_start);
    menu_system->addSeparator();
    menu_system->addAction(action_show);
    menu_system->addAction(action_hide);
    menu_system->addSeparator();
    menu_system->addAction(action_exit);
}

void MainWindow::create_toolbar()
{

    QLabel *label_find = new QLabel(tr(LINEEDIT_FIND));
    toolbar_edit_find = new QLineEdit;

    QLabel *label_depth = new QLabel(tr(LINEEDIT_DEPTH));
    toolbar_edit_depth = new QLineEdit(tr("3"));
    toolbar_edit_depth->setMinimumWidth(40);
    toolbar_edit_depth->setMaximumWidth(40);

    QLabel *label_time_proxy = new QLabel(tr(LINEEDIT_TIME_PROXY));
    toolbar_edit_time_proxy = new QLineEdit(tr("60"));
    toolbar_edit_time_proxy->setMinimumWidth(40);
    toolbar_edit_time_proxy->setMaximumWidth(40);

    //QLabel *label_time_wait = new QLabel(tr(LINEEDIT_TIME_OPEN));
    toolbar_edit_time_wait = new QLineEdit(tr("5"));
    toolbar_edit_time_wait->setMinimumWidth(40);
    toolbar_edit_time_wait->setMaximumWidth(40);

    QToolBar *toolbar_find = new  QToolBar;
    toolbar_find->setContextMenuPolicy(Qt::CustomContextMenu);//去掉右键菜单
    toolbar_find->setMovable(false);

    toolbar_find->addWidget(label_find);
    toolbar_find->addWidget(toolbar_edit_find);
    toolbar_find->addSeparator();

    toolbar_find->addWidget(label_depth);
    toolbar_find->addWidget(toolbar_edit_depth);
    toolbar_find->addSeparator();

    toolbar_find->addWidget(label_time_proxy);
    toolbar_find->addWidget(toolbar_edit_time_proxy);
    toolbar_find->addSeparator();

    //toolbar_find->addWidget(label_time_wait);
    //toolbar_find->addWidget(toolbar_edit_time_wait);
    //toolbar_find->addSeparator();

    toolbar_find->addAction(action_find);
    toolbar_find->addSeparator();
    toolbar_find->addAction(action_hide);
    toolbar_find->addSeparator();

    addToolBar(toolbar_find);


    QLabel *label2_word = new QLabel(tr(LINEEDIT_LINK_WORD));
    toolbar2_edit_word = new QLineEdit;
    connect(toolbar2_edit_word, SIGNAL(textChanged(QString)), this, SLOT(signal_edit_word(QString)));

    QLabel *label2_url = new QLabel(tr(LINEEDIT_LINK_URL));
    toolbar2_edit_url = new QLineEdit;
    connect(toolbar2_edit_url, SIGNAL(textChanged(QString)), this, SLOT(signal_edit_url(QString)));

    QToolBar *toolbar2 = new QToolBar;
    toolbar2->setContextMenuPolicy(Qt::CustomContextMenu);//去掉右键菜单
    toolbar2->setMovable(false);

    toolbar2->addWidget(label2_word);
    toolbar2->addWidget(toolbar2_edit_word);
    toolbar2->addSeparator();

    toolbar2->addWidget(label2_url);
    toolbar2->addWidget(toolbar2_edit_url);
    toolbar2->addSeparator();

    toolbar2->addAction(action_start);
    toolbar2->addSeparator();

    QDockWidget *dock_toolbar2 = new QDockWidget;
    dock_toolbar2->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock_toolbar2->setTitleBarWidget(toolbar2);//去掉标题栏
    addDockWidget(Qt::TopDockWidgetArea, dock_toolbar2);

    toolbar2_completer_word = NULL;
    toolbar2_completer_url = NULL;
}

void MainWindow::create_window()
{

    baidu = new WebBaidu;
    baidu->edit_info->setReadOnly(true);//设置只读
    baidu->edit_pack->setReadOnly(true);
    keyword = new KeyWord(baidu->edit_info);
    connect(keyword, SIGNAL(signal_keyword_end()), this, SLOT(signal_keyword()));

    QTabWidget *show_tab = new QTabWidget;
    show_tab->addTab(baidu->edit_info, tr("信息窗口"));
    show_tab->addTab(baidu->edit_pack, tr("报文窗口"));
    show_tab->addTab(baidu, tr("监视窗口"));

    //setCentralWidget(show_tab);//加入到中心窗口
    //setWindowFlags(Qt::FramelessWindowHint);//隐藏头部
    setContextMenuPolicy(Qt::CustomContextMenu);//去掉右键菜单
    setWindowIcon(QIcon(tr(":/images/system.png")));

    proxy_index = 0;
    proxy_color_start.setRgb(0, 255, 0);
    proxy_color_default.setRgb(255, 255, 255);
    connect(&proxy_time, SIGNAL(timeout()), this, SLOT(proxy_start_run()));

    QLineEdit *proxy_edit = new QLineEdit(tr("代理过滤"));
    connect(proxy_edit, SIGNAL(textChanged(QString)), this, SLOT(signal_edit_proxy(QString)));

    QVBoxLayout *layout_proxy = new QVBoxLayout;
    layout_proxy->addWidget(proxy_edit);
    layout_proxy->addWidget(&proxy_list);
    QWidget *proxy_widget = new QWidget;
    proxy_widget->setLayout(layout_proxy);

    QSplitter *layout_main = new QSplitter;
    layout_main->addWidget(proxy_widget);
    layout_main->addWidget(show_tab);

    setCentralWidget(layout_main);



#if 0
    QDockWidget *dock_proxy_edit = new QDockWidget;
    dock_proxy_edit->setFeatures(QDockWidget::DockWidgetMovable);
    dock_proxy_edit->setTitleBarWidget(proxy_edit);
    addDockWidget(Qt::LeftDockWidgetArea, dock_proxy_edit);
#endif

}

void MainWindow::create_status()
{
    statusBar()->showMessage(tr("status"));
}

void MainWindow::create_tray()
{

    QMenu *tray_menu = new QMenu;

    tray_menu->addAction(action_find);
    tray_menu->addAction(action_start);
    tray_menu->addSeparator();

    tray_menu->addAction(action_show);
    tray_menu->addAction(action_hide);
    tray_menu->addSeparator();

    tray_menu->addAction(action_exit);

    QSystemTrayIcon *tray = new QSystemTrayIcon;
    tray->setIcon(QIcon(tr(":/images/system.png")));
    tray->setContextMenu(tray_menu);
    tray->show();

    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(tray_click(QSystemTrayIcon::ActivationReason)));

    //退出在最后窗口关闭点击关闭时程序不关闭
    //QApplication::setQuitOnLastWindowClosed(false);
}

void MainWindow::tray_click(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {

    case QSystemTrayIcon::Trigger:
        //tray->showMessage("单击", "单击", QSystemTrayIcon::MessageIcon(), 5000);
        this->show();
        break;

    case QSystemTrayIcon::DoubleClick:
        //tray->showMessage("双击", "双击", QSystemTrayIcon::MessageIcon(), 5000);
        this->show();
        break;

    case QSystemTrayIcon::MiddleClick:
        break;

    default:break;

    }
}

void MainWindow::signal_keyword()
{
    action_start->setEnabled(true);
    toolbar2_edit_word->setEnabled(true);
    toolbar2_edit_url->setEnabled(true);

    if (toolbar2_completer_word) toolbar2_completer_word->deleteLater();
    if (toolbar2_completer_url) toolbar2_completer_url->deleteLater();
    toolbar2_completer_word = new QCompleter(keyword->list_word);
    toolbar2_completer_url = new QCompleter(keyword->list_url);
    toolbar2_edit_word->setCompleter(toolbar2_completer_word);
    toolbar2_edit_url->setCompleter(toolbar2_completer_url);

    save_config_file();
}

void MainWindow::signal_edit_word(QString str)
{
    for (int n = 0; n < keyword->list_word.size(); n++) {

        if (str == keyword->list_word.at(n)) {

            toolbar2_edit_url->setText(keyword->list_url.at(n));
            return;
        }
    }
}

void MainWindow::signal_edit_url(QString str)
{
    for (int n = 0; n < keyword->list_url.size(); n++) {

        if (str == keyword->list_url.at(n)) {

            toolbar2_edit_word->setText(keyword->list_word.at(n));
            return;
        }
    }
}

void MainWindow::signal_edit_proxy(QString str)
{

    proxy_list.clear();
    QStringList temp_list;

    foreach (QString item, proxy_str_list) {

        if (item.indexOf(str) != -1)
            temp_list.append(item);
    }

    proxy_list.addItems(temp_list);

#if 0
    QList<QListWidgetItem *> list;
    list = proxy_list.findItems(tr("*"), Qt::MatchWildcard | Qt::MatchRecursive);

    foreach (QListWidgetItem *item, list) {

        if (item->text().indexOf(str) == -1)
            proxy_list.removeItemWidget(item);
        else
            baidu->edit_info->append(item->text());
    }
#endif

}

void MainWindow::action_find_run()
{
    action_start->setEnabled(false);
    toolbar2_edit_word->setEnabled(false);
    toolbar2_edit_url->setEnabled(false);
    keyword->keyword_start(toolbar_edit_find->text(), toolbar_edit_depth->text().toInt());
}

void MainWindow::action_start_run(bool val)
{
    if (!val) {

        action_start->setText(tr(ACTION_START_START));
        proxy_time.stop();
        return;
    }

    action_start->setText(tr(ACTION_START_STOP));
    proxy_time.start(toolbar_edit_time_proxy->text().toInt() * 1000);
    baidu->link_url = toolbar2_edit_url->text();
    baidu->link_word = toolbar2_edit_word->text();
    baidu->find_word = toolbar_edit_find->text();
    baidu->web_start();

    save_config_file();
}

void MainWindow::proxy_start_run()
{

    QListWidgetItem *item = proxy_list.item(proxy_index);
    item->setBackgroundColor(proxy_color_start);

    QString str = item->text();
    QString addr = str.mid(0, str.indexOf("["));
    QString port = addr.section(':', 1, 1);
    addr.chop(port.size() + 1);

    baidu->stop();
    baidu->web_start(addr, port.toInt());

    if (proxy_index) {

        item = proxy_list.item(proxy_index - 1);
        item->setBackgroundColor(proxy_color_default);

    } else {

        item = proxy_list.item(proxy_list.count() - 1);
        item->setBackgroundColor(proxy_color_default);
    }

    if(++proxy_index >= proxy_list.count()) proxy_index = 0;
}



