#ifndef TIMECLIENT_H
#define TIMECLIENT_H
#include <QTcpSocket>

class TimeClient : public QObject
{
    Q_OBJECT
public:
    TimeClient(QObject * parent = 0);

    void connectToServer(const QString &server, qint16 port=37);

signals:
    void time(QString strTime);

private slots:
    void onConnected();
    void onReadyRead();

private:
    QTcpSocket * m_socket;
};
#endif // TIMECLIENT_H
