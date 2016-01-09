#include "widget.h"
#include <QApplication>
#include "timeService.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TimeServer server;
    server.bind(QHostAddress::Any, 37);

    Widget w;
    w.show();

    return a.exec();
}
