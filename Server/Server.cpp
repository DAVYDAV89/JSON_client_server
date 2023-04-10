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
#include <algorithm>

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
    for (const auto &el : m_Sockets )
        el->close();

    m_Sockets.clear();
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket* _socket = new QTcpSocket;
    _socket -> setSocketDescriptor(socketDescriptor);

    connect(_socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(_socket, &QTcpSocket::disconnected, this, &Server::slotCloseConnection);

    m_Sockets.push_back(_socket);
    qDebug() << "client connected " << socketDescriptor;
    sendToClient();
}

void Server::slotReadyRead()
{
    QTcpSocket* _socket = static_cast<QTcpSocket*>(sender());

    QFile file("phoneBook.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream( &file );
        stream << _socket -> readAll();
        file.close();
    }
    sendToClient();
}

void Server::sendToClient()
{
    QString strJS;
    QFile file("phoneBook.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        strJS = file.readAll();
        file.close();
    }

    for (const auto &el : m_Sockets )
        el->write(strJS.toLocal8Bit());
}

void Server::slotCloseConnection()
{
    QTcpSocket* _socket = static_cast<QTcpSocket*>(sender());
    _socket->close();
}
