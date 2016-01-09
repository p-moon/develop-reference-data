#ifndef TIMECLIENT_H
#define TIMECLIENT_H
#include <QUdpSocket>

class TimeClient : public QObject
{
    Q_OBJECT
public:
    TimeClient(QString server, QObject * parent = 0);

signals:
    void time(QString strTime);

private slots:
    void onReadyRead();

private:
    QUdpSocket * m_socket;
};
#endif // TIMECLIENT_H
