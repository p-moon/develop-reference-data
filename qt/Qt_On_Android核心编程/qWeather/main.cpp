#include "widget.h"
#include <QApplication>
#include "../qDebug2Logcat.h"
#include "../simpleCustomEvent.h"
#include <QDebug>
#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <jni.h>
#endif

#ifdef Q_OS_ANDROID
QObject *g_listener = 0;

static void onNetworkStateChanged(JNIEnv *env, jobject thiz,int state)
{
    qDebug() << "onNetworkStateChanged - " << state;
    if(g_listener)
    QCoreApplication::postEvent(g_listener, new SimpleCustomEvent(state == 1 ? 2: -2));
}

// result: RESULT_OK(-1),RESULT_CANCLED(0)
static void onLocationReady(JNIEnv *env, jobject thiz,int result, double longitude, double latitude)
{
    qDebug() << "onLocationReady, result - " << result
             << " longtitude - " << longitude
             << " latitude - " << latitude;
    QString info = QString("%1,%2").arg(longitude).arg(latitude);
    QCoreApplication::postEvent(g_listener, new SimpleCustomEvent(result == -1 ? 1: -1, info));
}

static bool registerNativeMethods()
{
    JNINativeMethod methods[] {
        {"OnLocationReady", "(IDD)V", (void*)onLocationReady},
        {"OnNetworkStateChanged", "(I)V", (void*)onNetworkStateChanged}
    };

    const char *classname = "an/qt/qWeather/QWeatherNative";
    jclass clazz;
    QAndroidJniEnvironment env;

    QAndroidJniObject javaClass(classname);
    clazz = env->GetObjectClass(javaClass.object<jobject>());
    //clazz = env->FindClass(classname);
    qDebug() << "find QWeatherNative - " << clazz;
    bool result = false;
    if(clazz)
    {
        jint ret = env->RegisterNatives(clazz,
                                        methods,
                                        sizeof(methods) / sizeof(methods[0]));
        env->DeleteLocalRef(clazz);
        qDebug() << "RegisterNatives return - " << ret;
        result = ret >= 0;
    }
    if(env->ExceptionCheck()) env->ExceptionClear();
    return result;
}

#endif

#define GLOBAL_STYLES \
    "QWidget {"\
    "  background-color: #555555;"\
    "  color: #F0F0F0;"\
    "}"\
    "QPushButton {"\
    "    border: 0px;"\
    "    border-radius: 8px;"\
    "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #50555f, STOP:0.5 #1c1e23, stop:1 #50555f);"\
    "    min-width: 66px;"\
    "    min-height: 30px;"\
    "}"\
    "QPushButton:pressed {"\
    "    color: #2222F0;"\
    "}" \
    "QLabel {"\
    "  min-height: 30px;"\
    "}"

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("qWeather");
    QApplication a(argc, argv);
    a.setStyleSheet(GLOBAL_STYLES);
    SimpleCustomEvent::eventType();
#ifdef Q_OS_ANDROID
    registerNativeMethods();
#endif
    Widget w;
#ifdef Q_OS_ANDROID
    g_listener = (QObject*)&w;
#endif
    w.show();

    return a.exec();
}
