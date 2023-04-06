#include "Server.h"
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QDialog>
#include <QtAlgorithms>
#include <QDebug>

Server::Server()
{
    if (listen(QHostAddress::Any, 3333)) {
        qDebug() << "start";
    }
    else {
        qDebug() << "error";
    }
    m_blockSize = 0;
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

    QDataStream _in(m_socket);
    _in.setVersion(QDataStream::Qt_6_2);
    if (_in.status() == QDataStream::Ok) {
        for(;;){
            if (m_blockSize == 0) {
                if (m_socket->bytesAvailable() < 2)
                    break;
                _in >> m_blockSize;
            }
            if (m_socket->bytesAvailable() < m_blockSize)
                break;

            QString _str;
            QByteArray _data;
            QImage     _image;

            _in >> _str;
            _in >> _data;

            m_blockSize = 0;

            if (_data.size() > 0) {

                QBuffer _buffer(&_data);
                _image.loadFromData(_data,"PNG");
                convertToMono(_image);
                _image.save(&_buffer, "PNG");
                sendToClient(_str + "\nSend file", _data);
            }
            else
                sendToClient(_str, _data);
            break;
        }
    }
}

void Server::sendToClient(QString _mess, const QByteArray &_ba)
{
    QByteArray _data;
    QDataStream out(&_data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);

    QByteArray _mediana;
    for(int i=0; i<_ba.size(); i = i + 3) {
        _mediana.push_back(_ba[0]);
        _mediana.push_back(middle_of_3(_ba[i], _ba[i+1], _ba[i+2]));
        _mediana.push_back(_ba[0]);
    }

//    std::sort(_mess.begin(), _mess.end());

    out << quint64(0);
    out << _mess;
    out << _ba;
    out.device() -> seek(0);
    out << quint64( _data.size() - sizeof (quint64) );

    for (const auto &el : m_Sockets )
        el->write(_data);

}

void Server::convertToMono(QImage & _img)
{
    for( int w = 0; w < _img.rect().right(); w++ ) {
        for( int h = 0; h < _img.rect().bottom(); h++ ) {
            QColor col( _img.pixel(w,h) );
            col.setHsv(col.hue(), 0, col.value(), col.alpha());
            _img.setPixel(w,h,col.rgb());
        }
    }
}

uint16_t Server::middle_of_3(uint16_t a, uint16_t b, uint16_t c)
{
   uint16_t middle;

   if ((a <= b) && (a <= c)){
      middle = (b <= c) ? b : c;
   }
   else{
      if ((b <= a) && (b <= c)){
         middle = (a <= c) ? a : c;
      }
      else{
         middle = (a <= b) ? a : b;
      }
   }

   return middle;
}

