#ifndef TIMETHREAD_H
#define TIMETHREAD_H

#include <QThread>
#include <QtNetwork>
#include <QTcpSocket>

class TimeThread : public QThread
{
    Q_OBJECT
public:
    TimeThread(int socketDescriptor,QObject *parent=0);
    void run();
signals:
    void error(QTcpSocket::SocketError socketError);
private:
    int socketDescriptor;
};

#endif // TIMETHREAD_H
