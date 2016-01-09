#include "widget.h"
#include <QApplication>
#include "../qDebug2Logcat.h"

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("touchDump");
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
