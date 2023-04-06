#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QBuffer>


class Server
        : public QTcpServer
{
    Q_OBJECT

    QTcpSocket              *m_socket;
    QVector<QTcpSocket*>    m_Sockets;    

public:
    Server();
    ~Server();

protected:
    void incomingConnection(qintptr);
private:
    void sendToClient(QString _mess, const QByteArray &_ba);
private slots:
    void slotReadyRead();
};

#endif // SERVER_H
