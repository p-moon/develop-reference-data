#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"
#include "../qDebug2Logcat.h"

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("qnote");
    QApplication a(argc, argv);
    //a.setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, false);

    QFont f = a.font();
    f.setPixelSize(20);
    a.setFont(f);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    MainWindow w;
    w.setAutoFillBackground(false);
    w.setWindowTitle("qnote");
#ifndef ANDROID
    w.resize(640,480);
#endif
    w.show();
    return a.exec();
}
