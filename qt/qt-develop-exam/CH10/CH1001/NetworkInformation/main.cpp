#include "networkinformation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetworkInformation w;
    w.show();
    
    return a.exec();
}
