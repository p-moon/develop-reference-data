#ifndef TIMESERVICE_H
#define TIMESERVICE_H
#include <QTcpServer>

class TimeServer : public QTcpServer
{
    Q_OBJECT
public:
    TimeServer(QObject * parent = 0);

protected slots:
    void onNewConnection();

protected:
    void sendTime(QTcpSocket *socket);
/*
private:
    void incomingConnection(int sock);
    */
};

#endif // TIMESERVICE_H
