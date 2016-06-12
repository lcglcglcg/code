#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpSocket>
#include <QMessageBox>
#include "lg_packet.h"
#include "task_table.h"
#include "chat_window.h"


class network_tcp : public QTcpSocket
{

    Q_OBJECT

public:
    network_tcp();

    void packet_analysis(char *buffer, int size);

    void packet_login(void);
    void packet_login_ret(char *buffer, int size);

    void packet_keyword_insert(QString keyword, QString url, int time_click[]);
    void packet_keyword_insert_ret(char *buffer, int size);

    void packet_keyword_update(QString keyword, int time_click[]);
    void packet_keyword_update_ret(char *buffer, int size);

    void packet_keyword_delete(QString keyword);
    void packet_keyword_delete_ret(char *buffer, int size);

    void packet_keyword_download(int index);
    void packet_keyword_download_ret(char *buffer, int size);
    
signals:
    void packet_signal_login(int retval);
    
public slots:
    void network_connect(void);
    void network_send(char *buffer, int size);
    void network_recv(void);
    void network_error(QAbstractSocket::SocketError socketError);
    void network_set_task_window(task_table *task_window);
    void network_set_chat_window(chat_widget *chat_window);

public:    
    QString account;
    QString password;
    task_table *task_window;
    chat_widget *chat_window;
};

#endif // NETWORK_H
