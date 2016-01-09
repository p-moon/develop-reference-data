#include "doorbell.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Master w;
    w.show();

    return a.exec();
}
