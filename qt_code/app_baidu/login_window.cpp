#include "login_window.h"

login_window::login_window(network_tcp *network)
{

    retval = -1;
    this->network = network;
    connect(network, SIGNAL(packet_signal_login(int)),
            this, SLOT(recv_signal_login(int)));

    setMinimumSize(200, 500);
    setMaximumSize(200, 500);

    QLabel *label_account = new QLabel(tr("用户名:"));
    QLabel *label_password = new QLabel(tr("密码:"));
    edit_account = new QLineEdit(tr("lichunguang"));
    edit_password = new QLineEdit(tr("lichunguang"));

    QPushButton *button_login = new QPushButton(tr("登录"));
    QPushButton *button_register = new QPushButton(tr("注册"));
    connect(button_login, SIGNAL(clicked()), this, SLOT(button_login_run()));
    connect(button_register, SIGNAL(clicked()), this, SLOT(button_register_run()));


    QVBoxLayout *layout_input = new QVBoxLayout;
    layout_input->addStretch();
    layout_input->addWidget(label_account);
    layout_input->addWidget(edit_account);
    layout_input->addWidget(label_password);
    layout_input->addWidget(edit_password);
    layout_input->addWidget(button_login);
    layout_input->addWidget(button_register);


    QLabel *label_blank = new QLabel;
    QGridLayout *layout = new QGridLayout(this);//整体布局
    layout->addWidget(label_blank, 0, 0);
    layout->addLayout(layout_input, 1, 0, 1, 0);
}

void login_window::paintEvent(QPaintEvent *)
{
    QPixmap img(tr(":/images/login.bmp"));
    QPainter painter(this);;
    painter.drawPixmap(0, 0, 200, 500, img);
}

void login_window::button_login_run(void)
{

    network->account = edit_account->text();
    network->password = edit_password->text();
    network->packet_login();
}

void login_window::button_register_run()
{

}

void login_window::recv_signal_login(int retval)
{

    if (retval == PACKET_SUCCESS) {

        this->retval = 0;
        this->close();
        return;
    }

    QMessageBox mess;
    mess.setText(tr("用户名密码错误!"));
    mess.exec();
}

int login_window::get_value() const
{
    return retval;
}
