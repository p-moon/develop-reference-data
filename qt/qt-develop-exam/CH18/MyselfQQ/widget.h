#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextCharFormat>
class QUdpSocket;
class Server;
namespace Ui {
class Widget;
}
enum MsgType{Msg, UsrEnter, UsrLeft, FileName, Refuse};
class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent,QString usrname);
    ~Widget();
protected:
    void usrEnter(QString usrname,QString ipaddr);
    void usrLeft(QString usrname,QString time);
    void sndMsg(MsgType type, QString srvaddr="");

    QString getIP();
    QString getUsr();
    QString getMsg();

    void hasPendingFile(QString usrname, QString srvaddr,QString clntaddr, QString filename);

    bool saveFile(const QString& filename);

    void closeEvent(QCloseEvent *);

private:
    Ui::Widget *ui;
    QUdpSocket *udpSocket;
    qint16 port;
    QString uName;

    QString fileName;
    Server *srv;

    QColor color;
private slots:
    void processPendingDatagrams();
    void on_sendBtn_clicked();

    void getFileName(QString);
    void on_sendTBtn_clicked();
    void on_fontCbx_currentFontChanged(const QFont &f);
    void on_sizeCbx_currentIndexChanged(const QString &arg1);
    void on_boldTBtn_clicked(bool checked);
    void on_italicTBtn_clicked(bool checked);
    void on_underlineTBtn_clicked(bool checked);
    void on_colorTBtn_clicked();

    void curFmtChanged(const QTextCharFormat &fmt);
    void on_saveTBtn_clicked();
    void on_clearTBtn_clicked();
    void on_exitBtn_clicked();
};

#endif // WIDGET_H
