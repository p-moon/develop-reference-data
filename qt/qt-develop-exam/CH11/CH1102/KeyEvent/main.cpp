#include "keyevent.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KeyEvent w;
    w.show();
    
    return a.exec();
}
