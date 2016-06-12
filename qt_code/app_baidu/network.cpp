#include "network.h"

network_tcp::network_tcp()
{

    this->task_window = NULL;
    this->chat_window = NULL;

    connect(this, SIGNAL(readyRead()), this, SLOT(network_recv()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(network_error(QAbstractSocket::SocketError)));

    network_connect();
}

void network_tcp::network_connect(void)
{

    connectToHost("onlinebaidu.ufstone.com", 5002);

    if (!waitForConnected()) {

        QMessageBox mess;
        mess.setText(tr("network_connect timeout!"));
        mess.exec();
    }
}

void network_tcp::network_send(char *buffer, int size)
{

    int retval = write(buffer, size);

    if (retval == -1 || retval == 0) {

        QMessageBox mess;
        mess.setText(tr("network_send error!"));
        mess.exec();
    }
}


void network_tcp::network_recv()
{


    char buffer[PACKET_BUFF_SIZE] = {0};
    int size = read(buffer, PACKET_BUFF_SIZE);

    if (size == -1 || size == 0) {

        QMessageBox mess;
        mess.setText(tr("network_recv error!"));
        mess.exec();
        return;
    }

    packet_analysis(buffer, size);
}

void network_tcp::network_error(QAbstractSocket::SocketError socketError)
{

    QMessageBox mess;

    switch (socketError) {

    case QAbstractSocket::RemoteHostClosedError:
        mess.setText(tr("服务器关闭!"));
        break;

    case QAbstractSocket::HostNotFoundError:
        mess.setText(tr("找不到服务器!"));
        break;

    case QAbstractSocket::ConnectionRefusedError:
        mess.setText(tr("服务器未开启!"));
        break;

    default:
        mess.setText(errorString());
        return;
    }

    //messbox.setText("是否退出?");
    //messbox.addButton(tr("是"), QMessageBox::AcceptRole);
    //messbox.addButton(tr("否"), QMessageBox::RejectRole);

    mess.setText(tr("网络异常 请重新登录!"));
    mess.exec();
    exit(-1);
}

void network_tcp::network_set_task_window(task_table *task_window)
{
    this->task_window = task_window;
}

void network_tcp::network_set_chat_window(chat_widget *chat_window)
{
    this->chat_window = chat_window;
}

void network_tcp::packet_analysis(char *buffer, int size)
{


    PACKET_HEAD *head = (PACKET_HEAD *)buffer;
    if (head->size != size) return;

    switch (head->msgid) {

    case PACKET_APP_HEARTBEAT_RET: //心跳返回
        return;

    case PACKET_APP_LOGIN_RET: //登录返回
        packet_login_ret(buffer, size);
        return;

    case PACKET_APP_KEYWORD_INSERT_RET: //关键词插入返回
        packet_keyword_insert_ret(buffer, size);
        return;

    case PACKET_APP_KEYWORD_UPDATE_RET: //关键词修改返回
        packet_keyword_update_ret(buffer, size);
        return;

    case PACKET_APP_KEYWORD_DOWNLOAD_RET: //下载关键词返回
        packet_keyword_download_ret(buffer, size);
        return;
    }

}

void network_tcp::packet_login(void)
{

    QByteArray account_byte = account.toUtf8();
    QByteArray password_byte = password.toUtf8();

    char *account_str = account_byte.data();
    char *password_str = password_byte.data();

    int account_size = account_byte.size();
    int password_size = password_byte.size();

    char buffer[PACKET_BUFF_SIZE] = {0};
    PACKET_APP_LOGIN_T *pack = (PACKET_APP_LOGIN_T *)buffer;
    pack->head.msgid = PACKET_APP_LOGIN;
    pack->head.size = sizeof(PACKET_APP_LOGIN_T) - sizeof(PACKET_UINT);

    int len = 0;
    char *data = (char *)&pack->data;

    pack->account_size = account_size;
    pack->password_size = password_size;

    memcpy(&data[len], account_str, account_size);
    len += account_size;

    memcpy(&data[len], password_str, password_size);
    len += password_size;

    pack->head.size += len;
    network_send(buffer, pack->head.size);
}

void network_tcp::packet_login_ret(char *buffer, int size)
{

    PACKET_APP_LOGIN_RET_T *pack = (PACKET_APP_LOGIN_RET_T *)buffer;
    if (pack->head.size != size) return;

    emit packet_signal_login(pack->retval);
}

void network_tcp::packet_keyword_insert(QString keyword, QString url, int time_click[])
{

    QByteArray keyword_byte = keyword.toUtf8();
    QByteArray url_byte = url.toUtf8();
    QByteArray account_byte = account.toUtf8();

    char *keyword_str = keyword_byte.data();
    char *url_str = url_byte.data();
    char *account_str = account_byte.data();

    int keyword_size = keyword_byte.size();
    int url_size = url_byte.size();
    int account_size = account_byte.size();

    char buffer[PACKET_BUFF_SIZE] = {0};
    PACKET_APP_KEYWORD_INSERT_T *pack = (PACKET_APP_KEYWORD_INSERT_T *)buffer;
    pack->head.msgid = PACKET_APP_KEYWORD_INSERT;
    pack->head.size = sizeof(PACKET_APP_KEYWORD_INSERT_T) - sizeof(PACKET_UINT);

    int len = 0;
    char *data = (char *)&pack->data;

    pack->keyword_size = keyword_size;
    pack->url_size = url_size;
    pack->account_size = account_size;
    memcpy(pack->click_time, time_click, sizeof(int) * 24);

    memcpy(&data[len], keyword_str, keyword_size);
    len += keyword_size;

    memcpy(&data[len], url_str, url_size);
    len += url_size;

    memcpy(&data[len], account_str, account_size);
    len += account_size;

    pack->head.size += len;
    network_send(buffer, pack->head.size);
}

void network_tcp::packet_keyword_insert_ret(char *buffer, int size)
{

    PACKET_APP_KEYWORD_INSERT_RET_T *pack = (PACKET_APP_KEYWORD_INSERT_RET_T *)buffer;
    if (pack->head.size != size) return;

    if (pack->retval == PACKET_ERROR) {

        QMessageBox mess;
        mess.setText(tr("关键词已存在!"));
        mess.exec();
        return;
    }

    packet_keyword_download(1);
}

void network_tcp::packet_keyword_update(QString keyword, int time_click[])
{
    QByteArray keyword_byte = keyword.toUtf8();
    QByteArray account_byte = account.toUtf8();

    char *account_str = account_byte.data();
    char *keyword_str = keyword_byte.data();

    int keyword_size = keyword_byte.size();
    int account_size = account_byte.size();

    char buffer[PACKET_BUFF_SIZE] = {0};
    PACKET_APP_KEYWORD_UPDATE_T *pack = (PACKET_APP_KEYWORD_UPDATE_T *)buffer;
    pack->head.msgid = PACKET_APP_KEYWORD_UPDATE;
    pack->head.size = sizeof(PACKET_APP_KEYWORD_UPDATE_T) - sizeof(PACKET_UINT);

    int len = 0;
    char *data = (char *)&pack->data;

    pack->keyword_size = keyword_size;
    pack->account_size = account_size;
    memcpy(pack->click_time, time_click, sizeof(int) * 24);

    memcpy(&data[len], keyword_str, keyword_size);
    len += keyword_size;

    memcpy(&data[len], account_str, account_size);
    len += account_size;

    pack->head.size += len;
    network_send(buffer, pack->head.size);
}

void network_tcp::packet_keyword_update_ret(char *buffer, int size)
{

    PACKET_APP_KEYWORD_UPDATE_RET_T *pack = (PACKET_APP_KEYWORD_UPDATE_RET_T *)buffer;
    if (pack->head.size != size) return;

    QMessageBox mess;

    if(pack->retval == PACKET_ERROR)
        mess.setText(tr("修改关键词错误 请重重新修改!"));
    else
        mess.setText(tr("修改关键完成!"));

    mess.exec();
}

void network_tcp::packet_keyword_delete(QString keyword)
{

    QByteArray keyword_byte = keyword.toUtf8();
    QByteArray account_byte = account.toUtf8();

    char *account_str = account_byte.data();
    char *keyword_str = keyword_byte.data();

    int keyword_size = keyword_byte.size();
    int account_size = account_byte.size();

    char buffer[PACKET_BUFF_SIZE] = {0};
    PACKET_APP_KEYWORD_DELETE_T *pack = (PACKET_APP_KEYWORD_DELETE_T *)buffer;
    pack->head.msgid = PACKET_APP_KEYWORD_DELETE;
    pack->head.size = sizeof(PACKET_APP_KEYWORD_DELETE_T) - sizeof(PACKET_UINT);

    int len = 0;
    char *data = (char *)&pack->data;

    pack->keyword_size = keyword_size;
    pack->account_size = account_size;

    memcpy(&data[len], keyword_str, keyword_size);
    len += keyword_size;

    memcpy(&data[len], account_str, account_size);
    len += account_size;

    pack->head.size += len;
    network_send(buffer, pack->head.size);
}

void network_tcp::packet_keyword_delete_ret(char *buffer, int size)
{

    PACKET_APP_KEYWORD_DELETE_RET_T *pack = (PACKET_APP_KEYWORD_DELETE_RET_T *)buffer;
    if (pack->head.size != size) return;

    char *data = (char *)&pack->data;

    char keyword_str[PACKET_BUFF_SIZE] = {0};
    memcpy(keyword_str, data, pack->keyword_size);

    task_window->task_table_del(keyword_str);
}

void network_tcp::packet_keyword_download(int index)
{

    QByteArray account_byte = account.toUtf8();
    char *account_str = account_byte.data();
    int account_size = account_byte.size();

    char buffer[PACKET_BUFF_SIZE] = {0};
    PACKET_APP_KEYWORD_DOWNLOAD_T *pack = (PACKET_APP_KEYWORD_DOWNLOAD_T *)buffer;
    pack->head.msgid = PACKET_APP_KEYWORD_DOWNLOAD;
    pack->head.size = sizeof(PACKET_APP_KEYWORD_DOWNLOAD_T) - sizeof(PACKET_UINT);

    char *data = (char *)&pack->data;

    pack->index = index;
    pack->account_size = account_size;

    memcpy(data, account_str, account_size);

    pack->head.size += account_size;
    network_send(buffer, pack->head.size);
}

void network_tcp::packet_keyword_download_ret(char *buffer, int size)
{

    PACKET_APP_KEYWORD_DOWNLOAD_RET_T *pack = (PACKET_APP_KEYWORD_DOWNLOAD_RET_T *)buffer;
    if (pack->head.size != size) return;
    if (pack->retval == PACKET_ERROR) return;

    int len = 0;
    char *data = (char *)&pack->data;

    int index = pack->index;

    char keyword_str[PACKET_BUFF_SIZE] = {0};
    memcpy(keyword_str, &data[len], pack->keyword_size);
    len += pack->keyword_size;

    char url_str[PACKET_BUFF_SIZE] = {0};
    memcpy(url_str, &data[len], pack->url_size);
    len += pack->url_size;

    if(task_window) {

        task_window->task_table_add(keyword_str, url_str);
        if (pack->retval != PACKET_COMPLETE)
            packet_keyword_download(++index);

        for (int n = 0; n < 24; n++)
            qDebug() << keyword_str << n << pack->click_time[n];
    }
}


