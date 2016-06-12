#include "lg_window.h"
#include <QApplication>

lg_tree *global_tree = NULL;
lg_edit *global_edit = NULL;

lg_window::lg_window()
{

    /***********初始化全局变量*****************/
    global_tree = &this->center_tree;
    global_edit = &this->center_edit;

    /***********中心布局*****************/
    center_layout.setOrientation((Qt::Horizontal));//左右布局
    center_layout.addWidget(&center_tree);
    center_layout.addWidget(&center_edit);
    center_layout.setStretchFactor(1, 1);
    setCentralWidget(&center_layout);
    connect(&center_tree, SIGNAL(itemPressed(QTreeWidgetItem*,int)), this, SLOT(tree_clicked(QTreeWidgetItem*,int)));
    connect(&center_tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tree_double_clicked(QModelIndex)));

    /***********tree右键菜单*****************/
    connect(action_data.tree_menu_dialog, SIGNAL(triggered()), this, SLOT(tree_menu_dialog_run()));
    connect(action_data.tree_menu_invite, SIGNAL(triggered()), this, SLOT(tree_menu_invite_run()));
    connect(action_data.tree_menu_adapter, SIGNAL(triggered()), this, SLOT(tree_menu_adapter_run()));}

lg_window::~lg_window()
{
}

void lg_window::tree_clicked(QTreeWidgetItem *item, int column)
{

    lg_tree_web_user *web_user = static_cast<lg_tree_web_user *>(item);

    //不是访客(web)
    if (web_user->text(1) != _TREE_WEB_USER_STRING) return;

    if (qApp->mouseButtons() == Qt::LeftButton) {

        //把此访客的聊天信息显示到show_edit
        center_edit.show_edit.setText(web_user->mess_buffer);

    } else if (qApp->mouseButtons() == Qt::RightButton) {

        action_data.tree_menu->exec(QCursor::pos());
    }
}

void lg_window::tree_double_clicked(QModelIndex index)
{

    lg_tree_web_user *web_user = static_cast<lg_tree_web_user *>(index.internalPointer());

    //不是访客(web)
    if (web_user->text(1) != _TREE_WEB_USER_STRING) return;


    if (web_user->text(2) == _TREE_WAIT_STRING) {

        //访客(web)在等待应答
        web_user->parent()->removeChild(web_user);//从父节点中删除
        web_user->setText(2, _TREE_DIALOG_STRING);//修改访客(web)状态
        center_tree.self.dialog.addChild(web_user);//转移到对话中

    } else if (web_user->text(2) == _TREE_VISIT_STRING) {

        //访客(web)在访问中
        web_user->parent()->removeChild(web_user);;//从父节点中删除
        web_user->setText(2, _TREE_INVITE_STRING);//修改访客(web)状态
        center_tree.self.invite.addChild(web_user);//转移到邀请中
    }
}

void lg_window::tree_menu_dialog_run()
{
    QMessageBox mess;
    QString str("dialog:");
    str += global_tree->currentItem()->text(0);
    mess.setText(str);
    mess.exec();
}

void lg_window::tree_menu_invite_run()
{
    QMessageBox mess;
    QString str("invite:");
    str += global_tree->currentItem()->text(0);
    mess.setText(str);
    mess.exec();
}
void lg_window::tree_menu_adapter_run()
{
    QMessageBox mess;
    QString str("adapter:");
    str += global_tree->currentItem()->text(0);
    mess.setText(str);
    mess.exec();
}

