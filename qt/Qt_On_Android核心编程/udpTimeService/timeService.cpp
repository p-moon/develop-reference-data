#include "timeService.h"
#include <QDateTime>
#include <QDebug>
#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

TimeServer::TimeServer(QObject *parent)
    : QUdpSocket(parent)
{
    connect(this, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
}

void TimeServer::onReadyRead()
{
    while( hasPendingDatagrams() )
    {
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());
        QHostAddress client;
        quint16 clientPort;

        readDatagram(datagram.data(), datagram.size(),
                                &client, &clientPort);

        uint now = QDateTime::currentDateTime().toTime_t();
        now = htonl((unsigned long)now);
        writeDatagram((const char *)&now, 4, client, clientPort);
    }
}
