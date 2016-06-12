#include "m_demo.h"

#define PACK_HTTP_GET   "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n"

m_demo::m_demo()
{

    connect(&sockfd, SIGNAL(readyRead()), this, SLOT(recv_run()));
    connect(&sockfd, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error_run(QAbstractSocket::SocketError)));

    connect_run();

    char *buffer = "GET /baidu_user.config  HTTP/1.1\r\nHost: 188.188.2.169:80\r\n\r\n";
    qint64 size = (strlen(buffer));

    append(buffer);
    append(QString::number(size));

    send_run(buffer, size);
}

void m_demo::connect_run()
{

    sockfd.connectToHost("188.188.2.169", 80);

    if (!sockfd.waitForConnected()) {

        mess.setText("connect time out");
        mess.exec();
    }
}

void m_demo::recv_run()
{

    qint64 size;
    char buffer[10240] = {0};

    size = sockfd.read(buffer, 10240);

    if (size == -1) {

        mess.setText("read error");
        mess.exec();
        return;
    }

    append(buffer);
}

void m_demo::send_run(char *buffer, qint64 buffer_size)
{

    qint64 size;

    size = sockfd.write(buffer, buffer_size);

    if (size == -1) {

        mess.setText("write error");
        mess.exec();
        return;
    }

    if (!sockfd.waitForBytesWritten()) {

        mess.setText("write time out");
        mess.exec();
        return;
    }
}

void m_demo::error_run(QAbstractSocket::SocketError socketError)
{

    switch (socketError) {

    case QAbstractSocket::RemoteHostClosedError:
        mess.setText("服务器close");
        break;

    case QAbstractSocket::HostNotFoundError:
        mess.setText("找不到服务器");
        break;

    case QAbstractSocket::ConnectionRefusedError:
        mess.setText("服务器拒绝连接");
        break;

    default:
        mess.setText(sockfd.errorString());
        break;
    }

    mess.exec();
}
