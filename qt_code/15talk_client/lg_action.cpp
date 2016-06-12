#include "lg_action.h"

lg_action::lg_action()
{

    tree_menu = new QMenu;
    tree_menu_dialog = new QAction(tr("dialog"), this);
    tree_menu_invite = new QAction(tr("invite"), this);
    tree_menu_adapter = new QAction(tr("adapter"), this);
    tree_menu->addAction(tree_menu_dialog);
    tree_menu->addAction(tree_menu_invite);
    tree_menu->addAction(tree_menu_adapter);
}

lg_action::~lg_action()
{

    delete tree_menu;
    delete tree_menu_dialog;
    delete tree_menu_invite;
    delete tree_menu_adapter;
}
