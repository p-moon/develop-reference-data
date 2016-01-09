#include "timeClient.h"
#include <QDateTime>
#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif


TimeClient::TimeClient(QObject *parent)
    : QObject(parent), m_socket(0)
{

}

void TimeClient::connectToServer(
        const QString & server, qint16 port)
{
    if(m_socket) return;
    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    m_socket->connectToHost(server, port);
}

void TimeClient::onConnected()
{
    m_socket->write("what\'s the time");
    qDebug() << "onConnected";
}

void TimeClient::onReadyRead()
{
    if(m_socket->bytesAvailable() < 4) return;
    uint now = 0;
    m_socket->read((char*)&now,4);
    QDateTime dateTime = QDateTime::fromTime_t(ntohl((unsigned long)now));
    qDebug() << dateTime;
    emit time(dateTime.toString("yyyy-MM-dd hh:mm:ss"));

    m_socket->close();
    m_socket->deleteLater();
    m_socket = 0;
}

