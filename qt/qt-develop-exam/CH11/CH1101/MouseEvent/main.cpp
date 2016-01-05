#include "mouseevent.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MouseEvent w;
    w.show();
    
    return a.exec();
}
