#ifndef LG_LOGIN_H
#define LG_LOGIN_H
#define NETWORK_BUFF_SIZE   40960

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QtNetwork>
#include <QMessageBox>

class lg_login : public QDialog
{
    Q_OBJECT

public:
    lg_login();
    ~lg_login();
    void paintEvent(QPaintEvent *);
    int get_value() const;

protected:
    int retval;//登陆成功返回1,失败返回0
    QMessageBox         mess_box;

    /***************布局***************/

    QLabel              label_blank;//占位
    QLabel              label_user;
    QLineEdit           edit_user;
    QLabel              label_pass;
    QLineEdit           edit_pass;
    QLabel              label_host;
    QComboBox           edit_host;

    QCheckBox           checkbox_login;
    QCheckBox           checkbox_pass;
    QHBoxLayout         *layout_checkbox;

    QPushButton         button_login;
    QPushButton         button_register;
    QPushButton         button_help;

    QVBoxLayout         *layout_data;//布局
    QGridLayout         *layout_main;//整体布局(占位+布局)

    /************scoket***********/

    QTcpSocket          sockfd;
    char                buffer[NETWORK_BUFF_SIZE];

protected slots:
    void network_connect();
    void network_recv();
    void network_send(char *buffer, qint64 size);
    void network_error(QAbstractSocket::SocketError socketError);
};

#endif // LG_LOGIN_H
