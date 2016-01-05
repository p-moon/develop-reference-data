#ifndef TIMECLIENT_H
#define TIMECLIENT_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QTcpSocket>
#include <QAbstractSocket>

class TimeClient : public QDialog
{
    Q_OBJECT
    
public:
    TimeClient(QWidget *parent = 0);
    ~TimeClient();
public slots:
    void enableGetBtn();
    void getTime();
    void readTime();
    void showError(QAbstractSocket::SocketError socketError);
private:
    QLabel *serverNameLabel;
    QLineEdit *serverNameLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QDateTimeEdit *dateTimeEdit;
    QLabel *stateLabel;

    QPushButton *getBtn;
    QPushButton *quitBtn;

    uint time2u;
    QTcpSocket *tcpSocket;
};

#endif // TIMECLIENT_H
