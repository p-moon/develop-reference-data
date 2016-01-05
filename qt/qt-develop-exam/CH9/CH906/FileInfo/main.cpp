#include "fileinfo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileInfo w;
    w.show();
    
    return a.exec();
}
