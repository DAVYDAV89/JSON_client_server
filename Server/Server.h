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

    quint64 m_blockSize;

public:
    Server();
    ~Server();

protected:
    void incomingConnection(qintptr);

private:
    void sendToClient(QString _mess, const QByteArray &_ba);
    void convertToMono(QImage &);

    uint16_t middle_of_3(uint16_t a, uint16_t b, uint16_t c);
private slots:
    void slotReadyRead();
};

#endif // SERVER_H
