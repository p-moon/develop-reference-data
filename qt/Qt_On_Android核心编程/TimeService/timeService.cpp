#include "timeService.h"
#include <QTcpSocket>
#include <QDateTime>
#include <QDebug>
#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

TimeServer::TimeServer(QObject *parent)
    : QTcpServer(parent)
{
    connect(this, SIGNAL(newConnection()),
            this, SLOT(onNewConnection()));
}

void TimeServer::onNewConnection()
{
    while(hasPendingConnections())
    {
        sendTime(nextPendingConnection());
    }
}

/*
void TimeServer::incomingConnection(int sock)
{
    TimeSocket * socket = new TimeSocket;
    connect(socket, SIGNAL(readyRead()), socket, SLOT(sendTime());
    socket->setSocketDescriptor(sock);
}
*/

void TimeServer::sendTime(QTcpSocket *sock)
{
    connect(sock, SIGNAL(disconnected()),
            sock, SLOT(deleteLater()));
    uint now = QDateTime::currentDateTime().toTime_t();
    now = htonl((unsigned long)now);
    sock->write((char*)&now, sizeof(now));
}
