#ifndef LG_WINDOW_H
#define LG_WINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QMessageBox>
#include "lg_tree.h"
#include "lg_edit.h"
#include "lg_action.h"

class lg_window : public QMainWindow
{
    Q_OBJECT

public:
    lg_window();
    ~lg_window();

    lg_tree             center_tree;//(界面中心)tree
    lg_edit             center_edit;//(界面中心)edit
    QSplitter           center_layout;//(界面中心)整体布局

public slots:
    void tree_clicked(QTreeWidgetItem *item, int column);
    void tree_double_clicked(QModelIndex index);
    void tree_menu_dialog_run();
    void tree_menu_invite_run();
    void tree_menu_adapter_run();

protected:
    lg_action           action_data;//action数据结构
    QMessageBox         mess_box;
};

#endif // LG_WINDOW_H
