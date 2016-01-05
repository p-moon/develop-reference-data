#include "mywidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyWidget w;
    w.setGeometry( 100, 100, 200, 120 );
    w.show();
    
    return a.exec();
}
