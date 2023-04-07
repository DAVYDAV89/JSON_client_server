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
    sendToClient();
}

void Server::slotReadyRead()
{
    m_socket = static_cast<QTcpSocket*>(sender());

    QFile file("phoneBook.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QVariantMap _map;
        QJsonDocument doc;

        QTextStream stream( &file );
        stream << m_socket -> readAll();
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
    m_socket->write(strJS.toLocal8Bit());
}
