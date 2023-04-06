#include "Server.h"
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QDialog>
#include <QtAlgorithms>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

Server::Server()
{
    if (listen(QHostAddress::Any, 9999)) {
        qDebug() << "start";
    }
    else {
        qDebug() << "error";
    }
}

Server::~Server()
{
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    m_socket = new QTcpSocket;
    m_socket -> setSocketDescriptor(socketDescriptor);

    connect(m_socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, m_socket, &Server::deleteLater);

    m_Sockets.push_back(m_socket);
    qDebug() << "client connected " << socketDescriptor;
}

void Server::slotReadyRead()
{
    m_socket = static_cast<QTcpSocket*>(sender());

    QByteArray _data;
    _data = m_socket -> readAll();

    QJsonDocument doc = QJsonDocument::fromJson(_data);
    QJsonObject json = doc.object();
    QJsonArray jarr = json["book"].toArray();

    foreach(const QJsonValue &val, jarr) {
        qDebug() << val["family"].toString();
        qDebug() << val["firstName"].toString();
        qDebug() << val["secondName"].toString();

        QString _lstNumber;
        foreach(const auto &number, val["phoneNumber"].toArray()) {
            _lstNumber.push_front(number.toString() + ",\n");
        }

        qDebug() << _lstNumber;
    }
}

void Server::sendToClient(QString _mess, const QByteArray &_ba)
{


}
