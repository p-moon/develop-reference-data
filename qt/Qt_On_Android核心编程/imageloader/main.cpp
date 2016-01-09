#include "widget.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont f = a.font();
    f.setPixelSize(60);
    a.setFont(f);
    Widget w;
    w.show();

    return a.exec();
}
