#include "dialog.h"

Dialog::Dialog()
{

    tray_icon_init();

    connect(&button_normal, SIGNAL(clicked()), this, SLOT(tray_icon_set_normal()));
    connect(&button_failure, SIGNAL(clicked()), this, SLOT(tray_icon_set_failure()));
    connect(&button_message, SIGNAL(clicked()), this, SLOT(tray_icon_message()));

    button_layout.addWidget(&button_normal);
    button_layout.addWidget(&button_failure);
    button_layout.addWidget(&button_message);
    setLayout(&button_layout);
}

Dialog::~Dialog()
{
    delete tray_icon_menu_normal;
    delete tray_icon_menu_failure;
    delete tray_icon_menu_quit;
}

void Dialog::tray_icon_init()
{

    tray_icon_normal = QIcon(":/images/fileopen.png");
    tray_icon_failure = QIcon(":/images/gotocell.png");

    tray_icon.setIcon(tray_icon_normal);
    setWindowIcon(tray_icon_normal);

    tray_icon_menu_normal = new QAction(tr("normal"), this);
    tray_icon_menu.addAction(tray_icon_menu_normal);
    connect(tray_icon_menu_normal, SIGNAL(triggered()), this, SLOT(tray_icon_set_normal()));

    tray_icon_menu_failure = new QAction(tr("failure"), this);
    tray_icon_menu.addAction(tray_icon_menu_failure);
    connect(tray_icon_menu_failure, SIGNAL(triggered()), this, SLOT(tray_icon_set_failure()));

    tray_icon_menu.addSeparator();//分隔符

    tray_icon_menu_quit = new QAction(tr("&Quit"), this);
    tray_icon_menu.addAction(tray_icon_menu_quit);
    connect(tray_icon_menu_quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    tray_icon.setContextMenu(&tray_icon_menu);
    tray_icon.show();
}

void Dialog::tray_icon_set_normal()
{
    tray_icon.setIcon(tray_icon_normal);
    setWindowIcon(tray_icon_normal);
}

void Dialog::tray_icon_set_failure()
{
    tray_icon.setIcon(tray_icon_failure);
    setWindowIcon(tray_icon_failure);
}

void Dialog::tray_icon_message()
{

    tray_icon.showMessage("new mess!", "new mess!",
                          QSystemTrayIcon::MessageIcon(), 5000);
}
