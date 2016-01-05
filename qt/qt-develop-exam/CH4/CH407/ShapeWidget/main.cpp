#include "shapewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShapeWidget w;
    w.show();
    
    return a.exec();
}
