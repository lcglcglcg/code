#include "lg_login.h"
#include "lg_pack_external.h"
#include <QPainter>

lg_login::lg_login()
{

    retval = 1;
    setWindowTitle(tr("login"));
    setMinimumSize(200, 500);
    setMaximumSize(200, 500);
    resize(200, 500);
    //setWindowFlags(Qt::WindowTitleHint);
    //setWindowFlags(Qt::FramelessWindowHint);

    //QPixmap img(":/images/login_img.bmp");
    //QLabel label_img;
    //label_img.setPixmap(img);
    //label_img.resize(img.height(), img.width());

    layout_checkbox = new QHBoxLayout;
    layout_data = new QVBoxLayout;
    layout_main = new QGridLayout(this);//整体布局(占位+布局)

    label_user.setText(tr("用户名"));
    layout_data->addWidget(&label_user);
    layout_data->addWidget(&edit_user);

    label_pass.setText(tr("密码"));
    layout_data->addWidget(&label_pass);
    layout_data->addWidget(&edit_pass);

    label_host.setText(tr("站点ID"));
    edit_host.setEditable(true);//可输入
    layout_data->addWidget(&label_host);
    layout_data->addWidget(&edit_host);

    checkbox_login.setText(tr("自动登录"));
    checkbox_pass.setText(tr("记住密码"));
    layout_checkbox->addWidget(&checkbox_login);
    layout_checkbox->addWidget(&checkbox_pass);
    layout_data->addLayout(layout_checkbox);

    button_login.setText(tr("登陆"));
    button_register.setText(tr("注册"));
    button_help.setText(tr("帮助"));
    layout_data->addWidget(&button_login);
    layout_data->addWidget(&button_register);
    layout_data->addWidget(&button_help);
    layout_data->addStretch();


    layout_main->addWidget(&label_blank, 0, 0);
    layout_main->addWidget(&label_blank, 1, 0);
    layout_main->addLayout(layout_data, 2, 0, 2, 0);
    //layout_main->setSizeConstraint(QLayout::SetFixedSize);
}

lg_login::~lg_login()
{

    delete layout_checkbox;
    delete layout_data;
    delete layout_main;
    sockfd.abort();
}

void lg_login::paintEvent(QPaintEvent *env)
{
    QPixmap img(tr(":/images/login_img.bmp"));
    QPainter painter(this);;
    painter.drawPixmap(0, 0, 200, 500, img);
}

int lg_login::get_value() const
{
    return retval;
}


void lg_login::network_connect()
{
    sockfd.connectToHost("188.188.2.162", 8888);

    if (!sockfd.waitForConnected())
        mess_box.setText("connect time out");
    else
        mess_box.setText("connect ok");

    mess_box.exec();
}

void lg_login::network_recv()
{
    qint64 size;
    memset(buffer, 0, NETWORK_BUFF_SIZE);
    size = sockfd.read(buffer, NETWORK_BUFF_SIZE);

    if (size == -1) {

        mess_box.setText("read error");
        mess_box.exec();
        return;
    }
}

void lg_login::network_send(char *buffer, qint64 size)
{
    qint64 retval;
    retval = sockfd.write(buffer, size);

    if (retval == -1) {

        network_connect();

    } else if (!sockfd.waitForBytesWritten()) {

        mess_box.setText("write time out");
        mess_box.exec();

    } else {

    }
}

void lg_login::network_error(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {

    case QAbstractSocket::RemoteHostClosedError:
        mess_box.setText(sockfd.errorString());
        break;//服务器主动close


    case QAbstractSocket::HostNotFoundError:
        mess_box.setText(sockfd.errorString());
        break;//找不到服务器


    case QAbstractSocket::ConnectionRefusedError:
        mess_box.setText(sockfd.errorString());
        break;//服务器拒绝连接

    default:
        mess_box.setText(sockfd.errorString());
        break;
    }

    mess_box.exec();
}
