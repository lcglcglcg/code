#ifndef M_DEMO_H
#define M_DEMO_H

#include <QtGui>
#include <QTcpSocket>

class m_demo : public QTextEdit
{

    Q_OBJECT

public:
    m_demo();
    
public slots:
    void connect_run();
    void recv_run();
    void send_run(char *buffer, qint64 buffer_size);
    void error_run(QAbstractSocket::SocketError socketError);

protected:
    QTcpSocket sockfd;
    QMessageBox mess;

};

#endif // M_DEMO_H
