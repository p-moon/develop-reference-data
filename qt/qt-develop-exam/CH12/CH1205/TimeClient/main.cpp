#include "timeclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimeClient w;
    w.show();
    
    return a.exec();
}
