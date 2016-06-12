#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QCompleter>
#include <QDockWidget>
#include <QListWidget>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QLayout>
#include <QSplitter>
#include <QMainWindow>
#include "baidu.h"
#include "keyword.h"


#define LINEEDIT_FIND               "搜索关键词:"
#define LINEEDIT_DEPTH              "搜索深度/页"
#define LINEEDIT_TIME_PROXY         "切换代理/秒:"
#define LINEEDIT_TIME_OPEN          "页面停留/秒:1-"
#define LINEEDIT_LINK_WORD          "连接关键词:"
#define LINEEDIT_LINK_URL           "URL:http://"

#define ACTION_FIND                 "分析关键词"
#define ACTION_START_START          "开始模拟"
#define ACTION_START_STOP           "暂停模拟"
#define ACTION_SHOW                 "显示窗口"
#define ACTION_HIDE                 "隐藏窗口"
#define ACTION_EXIT                 "退出程序"

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    MainWindow();
    int load_proxy_file();
    int load_config_file();
    int save_config_file();
    int load_keyword_file(int size);
    int save_keyword_file();

private:
    void create_action();
    void create_menus();
    void create_toolbar();
    void create_window();
    void create_status();
    void create_tray();

public slots:
    void tray_click(QSystemTrayIcon::ActivationReason reason);

    void signal_keyword();
    void signal_edit_word(QString str);
    void signal_edit_url(QString str);
    void signal_edit_proxy(QString str);

    void action_find_run();
    void action_start_run(bool val);
    void proxy_start_run();

private:
    QAction         *action_find;
    QAction         *action_start;
    QAction         *action_show;
    QAction         *action_hide;
    QAction         *action_exit;

    QLineEdit       *toolbar_edit_find;
    QLineEdit       *toolbar_edit_depth;
    QLineEdit       *toolbar_edit_time_proxy;
    QLineEdit       *toolbar_edit_time_wait;

    QLineEdit       *toolbar2_edit_word;
    QLineEdit       *toolbar2_edit_url;
    QCompleter      *toolbar2_completer_word;
    QCompleter      *toolbar2_completer_url;

    QListWidget     proxy_list;
    int             proxy_index;
    QStringList     proxy_str_list;
    QColor          proxy_color_start;
    QColor          proxy_color_default;


    QTimer          proxy_time;
    WebBaidu        *baidu;
    KeyWord         *keyword;

};

#endif // MAINWINDOW_H
