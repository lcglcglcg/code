#ifndef LG_TREE_H
#define LG_TREE_H

#define _TREE_WAIT_STRING           "等待应答"
#define _TREE_DIALOG_STRING         "对话中"
#define _TREE_INVITE_STRING         "邀请中"
#define _TREE_ADAPTE_STRING         "转接中"
#define _TREE_VISIT_STRING          "访问中"
#define _TREE_QUIT_STRING           "已离开"
#define _TREE_SELF_STRING           "自己"
#define _TREE_APP_USER_STRING       "APPUser"
#define _TREE_WEB_USER_STRING       "WebUser"

#define _TREE_WAIT                  1
#define _TREE_DIALOG                2
#define _TREE_INVITE                3
#define _TREE_ADAPTE                4
#define _TREE_VISIT                 5
#define _TREE_QUIT                  6

#define _TREE_COLOR_LEVEL_ONE       1//一级目录颜色
#define _TREE_COLOR_LEVEL_TWO       2//二级目录颜色
#define _TREE_COLOR_WEB_PAST        3//来过的访客颜色
#define _TREE_COLOR_WEB_OTHERS      4//其他的访客颜色

#include <QTreeWidgetItem>

/*
    QTreeWidgetItem->text数据结构

    text(0) = 显示的名称
    text(1) = 属性:客服(app)/访客(web)
    text(2) = 访客(web)状态
 */

//访客(web)数据结构
class lg_tree_web_user : public QTreeWidgetItem
{
public:
    lg_tree_web_user();
    QString mess_buffer;
};

//客服(app)数据结构
class lg_tree_app_user : public QTreeWidgetItem
{

public:
    lg_tree_app_user();
    QTreeWidgetItem     dialog;//对话中
    QTreeWidgetItem     invite;//邀请中
    QTreeWidgetItem     adapte;//转接中
};

//tree数据结构
class lg_tree : public QTreeWidget
{

public:
    lg_tree();
    void web_user_add(QString web_user_string, QString app_user_string, int mode);
    void web_user_add(QString web_user_string, int mode);
    void web_user_del(QString web_user_string);

    QTreeWidgetItem     wait;//等待应答
    lg_tree_app_user    self;//自己的列表
    QTreeWidgetItem     visit;//访问中
    QTreeWidgetItem     quit;//已离开

    /******************颜色相关****************************/

    QColor              level_one;//一级目录颜色
    QColor              level_two;//二级目录颜色
    QColor              level_past;//访问过的访客颜色
    QColor              level_others;//其他的访客颜色
};

#endif // LG_TREE_H
