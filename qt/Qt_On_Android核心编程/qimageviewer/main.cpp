#include "imagewidget.h"
#include <QApplication>
#include "../qDebug2Logcat.h"

int main(int argc, char *argv[])
{
    installLogcatMessageHandler("qimageviewer");
    QApplication a(argc, argv);
    QFont f = a.font();
    f.setPixelSize(20);
    a.setFont(f);

    ImageWidget w;
    w.show();

    return a.exec();
}
