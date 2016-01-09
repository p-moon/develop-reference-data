#include "chatWidget.h"
#include <QApplication>
#include "../qDebug2Logcat.h"

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("simpleRecorder");
    QApplication a(argc, argv);
    ChatWidget w;
    a.installEventFilter(&w);
    w.show();

    return a.exec();
}
