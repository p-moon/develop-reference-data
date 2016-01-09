#include "widget.h"
#include <QApplication>
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <jni.h>
#include <QDebug>
#include "../qDebug2Logcat.h"
#include "../simpleCustomEvent.h"
#include <QFile>
#include <QDir>

QObject *g_listener = 0;

static jstring getQtVersion(JNIEnv *env, jobject thiz)
{
    QString version(qVersion());
    QAndroidJniObject javaString = QAndroidJniObject::fromString(version);
    return javaString.object<jstring>();
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

jclass g_extendsNative = 0;
bool registerNativeMethods()
{
    JNINativeMethod methods[] {
        {"GetQtVersion","()Ljava/lang/String;", (void*)getQtVersion},
        {"OnLocationReady", "(IDD)V", (void*)onLocationReady},
        {"OnImageCaptured", "(ILjava/lang/String;)V", (void*)onImageCaptured}
    };

    const char *classname = "an/qt/extendsQtWithJava/ExtendsQtNative";
    jclass clazz;
    QAndroidJniEnvironment env;

    QAndroidJniObject javaClass(classname);
    clazz = env->GetObjectClass(javaClass.object<jobject>());
    //clazz = env->FindClass(classname);
    qDebug() << "find ExtendsQtNative - " << clazz;
    bool result = false;
    if(clazz)
    {
        //g_extendsNative = static_cast<jclass>(env->NewGlobalRef(clazz));
        jint ret = env->RegisterNatives(clazz,
                                        methods,
                                        sizeof(methods) / sizeof(methods[0]));
        env->DeleteLocalRef(clazz);
        qDebug() << "RegisterNatives return - " << ret;
        result = ret >= 0;
        /*
        QAndroidJniObject::callStaticMethod<void>("an/qt/extendsQtWithJava/ExtendsQtNative",
                                               "onImageCaptured", "(I)V",0);

        */
    }
    if(env->ExceptionCheck()) env->ExceptionClear();
    return result;
}

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("extendsQt");
    QApplication a(argc, argv);
    SimpleCustomEvent::eventType();

    registerNativeMethods();
    /*
    QAndroidJniObject path =
        QAndroidJniObject::callStaticObjectMethod(
                "an/qt/extendsQtWithJava/ExtendsQtWithJava",
                "getSdcardPath", "()Ljava/lang/String;");
    QString sdPath = path.toString();
    if(!sdPath.isEmpty())
    {
        QDir dir(sdPath);
        if( dir.mkdir("newdir") )
        {
            QFile file(QString("%1/newdir/qtfile.txt").arg(sdPath));
            if(file.open(QFile::WriteOnly))
            {
                file.setPermissions(QFile::ReadOwner |
                                    QFile::ReadGroup |
                                    QFile::ReadOther);
                file.write("test write file");
                file.flush();
                file.close();
            }
        }
    }
    */

    Widget w;
    g_listener = qobject_cast<QObject*>(&w);
    w.show();

    return a.exec();
}
