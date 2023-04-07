#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QBuffer>


class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();
    ~Server();

protected:
    void incomingConnection(qintptr);

private:

    QTcpSocket              *m_socket;
    QVector<QTcpSocket*>    m_Sockets;

    void sendToClient();

private slots:
    void slotReadyRead();
};

#endif // SERVER_H
