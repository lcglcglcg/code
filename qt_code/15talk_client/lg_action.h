#ifndef LG_ACTION_H
#define LG_ACTION_H

#include <QMenu>
#include <QAction>

class lg_action : public QObject
{
public:
    lg_action();
    ~lg_action();

    QMenu   *tree_menu;
    QAction *tree_menu_dialog;
    QAction *tree_menu_invite;
    QAction *tree_menu_adapter;
};

#endif // LG_ACTION_H
