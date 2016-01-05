#include "dockwindows.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DockWindows w;
    w.show();
    
    return a.exec();
}
