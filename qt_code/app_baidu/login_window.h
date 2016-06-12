#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPainter>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QtNetwork>
#include <QListWidget>
#include <QApplication>
#include <QMessageBox>
#include "network.h"


class login_window : public QDialog
{
    Q_OBJECT

public:
    login_window(network_tcp *network);
    void paintEvent(QPaintEvent *);
    int get_value() const;

public slots:
    void button_login_run(void);
    void button_register_run(void);
    void recv_signal_login(int retval);

public:
    int retval;
    QLineEdit *edit_account;
    QLineEdit *edit_password;
    network_tcp *network;
};

#endif // LOGIN_H
