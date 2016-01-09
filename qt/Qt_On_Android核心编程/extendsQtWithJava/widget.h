#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    bool event(QEvent *);

protected slots:
    void onConnectButton();
    void onToastButton();
    void onNotifyButton();
    void onActivityButton();
    void onBroadcastButton();
    void onAppStateChanged(Qt::ApplicationState state);
    void onGetLocation();
    void onCaptureImage();

private:
    void checkNetworkState();
    void startActivity(QString name);

private:
    QLabel * m_networkState;
    QPushButton *m_connectButton;
    QPushButton *m_toastButton;
    QLineEdit   *m_toastEdit;
    QPushButton *m_notifyButton;
    QLineEdit   *m_notificationEdit;
    QPushButton *m_broadcastButton;
    QLineEdit   *m_broadcastEdit;
    QPushButton *m_activityButton;
    QLineEdit   *m_activityEdit;
    QLabel *m_locationState;
    QLabel *m_captureState;
};

#endif // WIDGET_H
