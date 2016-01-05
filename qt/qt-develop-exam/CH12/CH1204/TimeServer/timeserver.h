#ifndef TIMESERVER_H
#define TIMESERVER_H

#include <QTcpServer>
class Dialog;                         //服务器端的声明
class TimeServer : public QTcpServer
{
    Q_OBJECT
public:
    TimeServer(QObject *parent=0);
protected:
    void incomingConnection(int socketDescriptor);
private:
    Dialog *dlg;
};

#endif // TIMESERVER_H
