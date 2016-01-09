#include "widget.h"
#include <QApplication>
#include "../qDebug2Logcat.h"
#include <QDebug>
#include "../simpleCustomEvent.h"
#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <jni.h>
#include <QFile>
#include <QDir>
#endif
#include <QScreen>

QString g_tempPath;
QString g_sdPath;
#ifdef Q_OS_ANDROID
QObject *g_listener = 0;
static void onImageCaptured(JNIEnv *env, jobject thiz,int result, jstring imageFile)
{
    QString image;
    const char *nativeString = env->GetStringUTFChars(imageFile, 0);
    image = nativeString;
    env->ReleaseStringUTFChars(imageFile, nativeString);
    qDebug() << "onImageCaptured, result - " << result << " image - " << image;
    int ret = result;
    if(result == -1 && QFile::exists(image))
    {
        ret = 2;
    }
    else
    {
        ret = -2;
        qDebug() << "could not read the captured image!";
    }
    QCoreApplication::postEvent(g_listener, new SimpleCustomEvent(ret, image));
}

static bool registerNativeMethods()
{
    JNINativeMethod methods[] {
        {"OnImageCaptured", "(ILjava/lang/String;)V", (void*)onImageCaptured}
    };

    const char *classname = "an/qt/imageProcessor/ImageCaptureNative";
    jclass clazz;
    QAndroidJniEnvironment env;

    QAndroidJniObject javaClass(classname);
    clazz = env->GetObjectClass(javaClass.object<jobject>());
    //clazz = env->FindClass(classname);
    qDebug() << "find ImageCaptureNative - " << clazz;
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

static void makeTempPath()
{
#ifdef Q_OS_ANDROID
    QAndroidJniObject path =
        QAndroidJniObject::callStaticObjectMethod(
                "an/qt/imageProcessor/ImageProcessorActivity",
                "getSdcardPath", "()Ljava/lang/String;");
    QString sdPath = path.toString();
    if(!sdPath.isEmpty())
    {
        g_sdPath = sdPath;
        QDir dir(sdPath);
        if( dir.exists("imageProcessor") || dir.mkdir("imageProcessor") )
        {
            g_tempPath = QString("%1/imageProcessor/").arg(sdPath);
        }
    }
#endif
    if(g_tempPath.isEmpty())
    {
        g_tempPath = QDir::currentPath();
    }
}

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("ImageProcessor");
    QApplication a(argc, argv);
    SimpleCustomEvent::eventType();
#ifdef Q_OS_ANDROID
    registerNativeMethods();
#endif
    makeTempPath();
    Widget w;
#ifdef Q_OS_ANDROID
    g_listener = (QObject*)&w;
#endif
    QScreen *screen = a.primaryScreen();
    w.setMaximumSize(screen->size());
    w.show();

    return a.exec();
}
