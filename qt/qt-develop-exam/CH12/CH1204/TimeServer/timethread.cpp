#include "timethread.h"
#include <QDateTime>
#include <QByteArray>
#include <QDataStream>

TimeThread::TimeThread(int socketDescriptor,QObject *parent)
    :QThread(parent),socketDescriptor(socketDescriptor)
{
}

void TimeThread::run()
{
    QTcpSocket tcpSocket;
    if(!tcpSocket.setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket.error());
        return;
    }
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_3);

    uint time2u = QDateTime::currentDateTime().toTime_t();
    out<<time2u;

    tcpSocket.write(block);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}
