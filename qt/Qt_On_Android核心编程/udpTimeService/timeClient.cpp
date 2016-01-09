#include "timeClient.h"
#include <QDateTime>
#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif


TimeClient::TimeClient(QString server, QObject *parent)
    : QObject(parent), m_socket(new QUdpSocket(this))
{
    m_socket->bind(QHostAddress::Any, 9653);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    m_socket->writeDatagram("what\'s the time", 15, QHostAddress(server), 37);
}
/*
void TimeClient::connectToServer(
        const QString & server, qint16 port)
{
    if(m_socket) return;
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    m_socket->connectToHost(server, port);
}
*/


void TimeClient::onReadyRead()
{
    uint now = 0;
    m_socket->readDatagram((char*)&now, 4);

    QDateTime dateTime = QDateTime::fromTime_t(ntohl((unsigned long)now));
    qDebug() << dateTime;
    emit time(dateTime.toString("yyyy-MM-dd hh:mm:ss"));

    m_socket->deleteLater();
    m_socket = 0;
}

