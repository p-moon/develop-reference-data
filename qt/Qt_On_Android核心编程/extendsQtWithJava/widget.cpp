#include "widget.h"
#include <QGridLayout>
#include <QtAndroidExtras/QAndroidJniObject>
#ifdef ANDROID
#include<unistd.h>
#endif
#include <QGuiApplication>
#include <QAndroidJniEnvironment>
#include <QDebug>
#include "../simpleCustomEvent.h"
#include <QPixmap>

// 1.系统设置
//    android.settings.SETTINGS
// 2.无线和网络设置，如 WIFI / 蓝牙 / 3G数据链接
//    android.settings.WIRELESS_SETTINGS
// 3.WIFI设置
//    android.settings.WIFI_SETTINGS


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    connect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)),
            this, SLOT(onAppStateChanged(Qt::ApplicationState)));
    QGridLayout *layout = new QGridLayout(this);
    layout->setColumnStretch(1, 1);

    m_networkState = new QLabel();
    layout->addWidget(m_networkState, 0, 0, 1, 2);
    m_connectButton = new QPushButton("Connect");
    connect(m_connectButton, SIGNAL(clicked()),
            this, SLOT(onConnectButton()));
    layout->addWidget(m_connectButton, 0, 2);
    checkNetworkState();

    layout->addWidget(new QLabel("Toast:"), 1, 0);
    m_toastEdit = new QLineEdit("Qt toast");
    layout->addWidget(m_toastEdit, 1, 1);
    m_toastButton = new QPushButton("makeToast");
    layout->addWidget(m_toastButton, 1, 2);
    connect(m_toastButton, SIGNAL(clicked()), this
            , SLOT(onToastButton()));

    layout->addWidget(new QLabel("通知:"), 2, 0);
    m_notificationEdit = new QLineEdit("Qt notification");
    layout->addWidget(m_notificationEdit, 2, 1);
    m_notifyButton = new QPushButton("Send");
    connect(m_notifyButton, SIGNAL(clicked()), this,
            SLOT(onNotifyButton()));
    layout->addWidget(m_notifyButton, 2, 2);

    layout->addWidget(new QLabel("广播:"), 3, 0);
    m_broadcastEdit = new QLineEdit("an.qt.extendsQt.test");
    layout->addWidget(m_broadcastEdit, 3, 1);
    m_broadcastButton = new QPushButton("Send");
    connect(m_broadcastButton, SIGNAL(clicked()),
            this, SLOT(onBroadcastButton()));
    layout->addWidget(m_broadcastButton, 3, 2);

    layout->addWidget(new QLabel("Activity:"), 4, 0);
    m_activityEdit = new QLineEdit("android.settings.SETTINGS");
    layout->addWidget(m_activityEdit, 4, 1);
    m_activityButton = new QPushButton("Start");
    connect(m_activityButton, SIGNAL(clicked()),
                      this, SLOT(onActivityButton()));
    layout->addWidget(m_activityButton, 4, 2);

    layout->addWidget(new QLabel("Location:"), 5, 0);
    m_locationState = new QLabel;
    layout->addWidget(m_locationState, 5, 1);
    QPushButton * btn = new QPushButton("Get");
    connect(btn, SIGNAL(clicked()),
            this, SLOT(onGetLocation()));
    layout->addWidget(btn, 5, 2);

    layout->addWidget(new QLabel("CaptureImage:"), 6, 0);
    m_captureState = new QLabel;
    layout->addWidget(m_captureState, 6, 1);
    btn = new QPushButton("camera");
    connect(btn, SIGNAL(clicked()), this, SLOT(onCaptureImage()));
    layout->addWidget(btn, 6, 2);

    layout->setRowStretch(7, 1);
}

Widget::~Widget()
{

}

bool Widget::event(QEvent *e)
{
    if(e->type() == SimpleCustomEvent::eventType())
    {
        SimpleCustomEvent *sce = (SimpleCustomEvent*)e;
        if(sce->m_arg1 == 1)
        {
            m_locationState->setText(sce->m_arg2);
        }
        else if(sce->m_arg1 == -1)
        {
            m_locationState->setText("error!");
        }
        else if(sce->m_arg1 == 2)
        {
            m_locationState->setPixmap(QPixmap(sce->m_arg2));
        }
        else
        {
            m_captureState->setText("cancel");
        }
        return true;
    }
    return QWidget::event(e);
}

void Widget::onConnectButton()
{
    startActivity("android.settings.WIRELESS_SETTINGS");
}

void Widget::onToastButton()
{
#ifdef ANDROID
    QAndroidJniObject javaToast = QAndroidJniObject::fromString(m_toastEdit->text());
    QAndroidJniObject::callStaticMethod<void>("an/qt/extendsQtWithJava/ExtendsQtWithJava",
                                       "makeToast",
                                       "(Ljava/lang/String;)V",
                                       javaToast.object<jstring>());
#endif
}

void Widget::onNotifyButton()
{
#ifdef ANDROID
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(m_notificationEdit->text());
    QAndroidJniObject::callStaticMethod<void>("an/qt/extendsQtWithJava/ExtendsQtWithJava",
                                       "notify",
                                       "(Ljava/lang/String;)V",
                                       javaNotification.object<jstring>());
#endif
}

void Widget::onBroadcastButton()
{
#ifdef ANDROID
    QAndroidJniObject javaAction = QAndroidJniObject::fromString(m_broadcastEdit->text());
    QAndroidJniObject javaExtra= QAndroidJniObject::fromString("sender_pid");
    QAndroidJniObject::callStaticMethod<void>("an/qt/extendsQtWithJava/ExtendsQtWithJava",
                                       "emitBroadcast",
                                       "(Ljava/lang/String;Ljava/lang/String;I)V",
                                       javaAction.object<jstring>(),
                                       javaExtra.object<jstring>(),
                                       getpid());
#endif
}

void Widget::onAppStateChanged(Qt::ApplicationState state)
{
    if(state == Qt::ApplicationActive)
    {
        checkNetworkState();
    }
}

void Widget::onGetLocation()
{
#ifdef ANDROID
    QAndroidJniObject::callStaticMethod<void>("an/qt/extendsQtWithJava/ExtendsQtWithJava",
                                       "getLocation");
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "Widget,onGetLocation, got exception";
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
#endif
}

void Widget::onCaptureImage()
{
#ifdef ANDROID
    QAndroidJniObject::callStaticMethod<void>("an/qt/extendsQtWithJava/ExtendsQtWithJava",
                                       "captureImage");
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "Widget,onCaptureImage, got exception";
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
#endif
}

void Widget::onActivityButton()
{
    startActivity(m_activityEdit->text());
}

void Widget::checkNetworkState()
{
#ifdef ANDROID

    jint state = QAndroidJniObject::callStaticMethod<jint>("an/qt/extendsQtWithJava/ExtendsQtWithJava",
                                       "networkState",
                                       "()I");
    m_networkState->setText(state == 1 ? "网络已连接":"网络尚未连接");
#endif
}

void Widget::startActivity(QString name)
{
#ifdef ANDROID
    QAndroidJniObject javaAction = QAndroidJniObject::fromString(name);
    QAndroidJniObject::callStaticMethod<void>("an/qt/extendsQtWithJava/ExtendsQtWithJava",
                                       "launchActivity",
                                       "(Ljava/lang/String;)V",
                                       javaAction.object<jstring>());
#endif
}
