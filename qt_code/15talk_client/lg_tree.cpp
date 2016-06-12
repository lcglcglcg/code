#include "lg_tree.h"

lg_tree_web_user::lg_tree_web_user()
{
    setText(1, _TREE_WEB_USER_STRING);//属性为访客(web)
}

lg_tree_app_user::lg_tree_app_user()
{

    addChild(&dialog);
    addChild(&invite);
    addChild(&adapte);

    dialog.setText(0, _TREE_DIALOG_STRING);
    invite.setText(0, _TREE_INVITE_STRING);
    adapte.setText(0, _TREE_ADAPTE_STRING);

    this->setText(1, _TREE_APP_USER_STRING);//属性为客服(app);
    dialog.setText(1, _TREE_APP_USER_STRING);//属性为客服(app);
    invite.setText(1, _TREE_APP_USER_STRING);//属性为客服(app);
    adapte.setText(1, _TREE_APP_USER_STRING);//属性为为客服(app);
}

lg_tree::lg_tree()
{
    setHeaderHidden(true);//隐藏QTreeWidget头信息
    setAlternatingRowColors(true);//显示网格

    /***tree字体大小***/
    QFont font;
    font.setPointSize(10);
    setFont(font);

    /**详细说明查看lg_tree.h QTreeWidgetItem->text数据结构**/

    wait.setText(0, _TREE_WAIT_STRING);
    addTopLevelItem(&wait);//添加到tree

    self.setText(0, _TREE_SELF_STRING);
    addTopLevelItem(&self);

    visit.setText(0, _TREE_VISIT_STRING);
    addTopLevelItem(&visit);

    quit.setText(0, _TREE_QUIT_STRING);
    addTopLevelItem(&quit);

    expandAll();//tree全部展开

    level_others.setRgb(255, 255, 255);//颜色设置(白色)
}

void lg_tree::web_user_add(QString web_user_string, QString app_user_string, int mode)
{

    QTreeWidgetItem *temp = NULL;
    lg_tree_web_user *web_user = NULL;
    lg_tree_app_user *app_user = NULL;
    QList<QTreeWidgetItem *> list;

    if (web_user_string == tr("") || app_user_string == tr(""))
        return;

    list = findItems(tr("*"), Qt::MatchWildcard | Qt::MatchRecursive);

    foreach (QTreeWidgetItem *item, list) {

        //访客(web)重复了
        if (item->text(0) == web_user_string)
            return;

        //查找客服(app)
        if (item->text(0) == app_user_string) {
            app_user = static_cast <lg_tree_app_user *>(item);
            break;
        }
    }

    if (!app_user) return;//没有此客服(app)

    switch (mode) {

    case _TREE_DIALOG:
        temp = &app_user->dialog;
        web_user = new lg_tree_web_user;
        web_user->setText(2, _TREE_DIALOG_STRING);//访客状态对话中
        break;

    case _TREE_INVITE:
        temp = &app_user->invite;
        web_user = new lg_tree_web_user;
        web_user->setText(2, _TREE_INVITE_STRING);//访客状态邀请中
        break;

    case _TREE_ADAPTE:
        temp = &app_user->adapte;
        web_user = new lg_tree_web_user;
        web_user->setText(2, _TREE_ADAPTE_STRING);//访客状态转接中
        break;

    default:return;

    }

    web_user->setText(0, web_user_string);//标题
    web_user->setText(1, _TREE_WEB_USER_STRING);//标记为访客(web)
    web_user->setBackground(0, level_others);//颜色
    temp->addChild(web_user);
}

void lg_tree::web_user_add(QString web_user_string, int mode)
{

    QTreeWidgetItem *temp = NULL;
    lg_tree_web_user *web_user = NULL;
    QList<QTreeWidgetItem *> list;

    if (web_user_string == tr("")) return;

    list = findItems(tr("*"), Qt::MatchWildcard | Qt::MatchRecursive);
    foreach (QTreeWidgetItem *item, list) {

        //访客(web)重复了
        if (item->text(0) == web_user_string)
            return;
    }

    switch (mode) {

    case _TREE_WAIT:
        temp = &wait;
        web_user = new lg_tree_web_user;
        web_user->setText(2, _TREE_WAIT_STRING);//访客状态等待应答
        break;

    case _TREE_DIALOG:
        temp = &self.dialog;
        web_user = new lg_tree_web_user;
        web_user->setText(2, _TREE_DIALOG_STRING);//访客状态对话中
        break;

    case _TREE_INVITE:
        temp = &self.invite;
        web_user = new lg_tree_web_user;
        web_user->setText(2, _TREE_INVITE_STRING);//访客状态邀请中
        break;

    case _TREE_ADAPTE:
        temp = &self.adapte;
        web_user = new lg_tree_web_user;
        web_user->setText(2, _TREE_ADAPTE_STRING);//访客状态转接中
        break;

    case _TREE_VISIT:
        temp = &visit;
        web_user = new lg_tree_web_user;
        web_user->setText(2, _TREE_VISIT_STRING);//访客状态访问中
        break;

    case _TREE_QUIT:
        temp = &quit;
        web_user = new lg_tree_web_user;
        web_user->setText(2, _TREE_QUIT_STRING);//访客状态已离开
        break;

    default:return;
    }

    web_user->setText(0, web_user_string);
    web_user->setText(1, _TREE_WEB_USER_STRING);//标记为访客(web)
    web_user->setBackground(0, level_others);
    temp->addChild(web_user);
}

void lg_tree::web_user_del(QString web_user_string)
{

    QTreeWidgetItem *temp;
    lg_tree_web_user *web_user = NULL;
    QList<QTreeWidgetItem *> list;

    if (web_user_string == tr("")) return;

    list = findItems(tr("*"), Qt::MatchWildcard | Qt::MatchRecursive);

    foreach (QTreeWidgetItem *item, list) {

        if (item->text(0) == web_user_string) {

            web_user = static_cast <lg_tree_web_user *>(item);
            break;
        }
    }

    if (web_user) {

        temp = web_user->parent();

        if (temp) {

            temp->removeChild(web_user);
            delete web_user;
        }
    }
}
