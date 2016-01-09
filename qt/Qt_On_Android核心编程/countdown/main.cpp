#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
#ifdef WIN32
    w.resize(400,150);
#endif
    w.show();
    return a.exec();
}
