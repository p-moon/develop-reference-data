#ifndef TIMESERVICE_H
#define TIMESERVICE_H
#include <QUdpSocket>

class TimeServer : public QUdpSocket
{
    Q_OBJECT
public:
    TimeServer(QObject * parent = 0);

protected slots:
    void onReadyRead();
};

#endif // TIMESERVICE_H
