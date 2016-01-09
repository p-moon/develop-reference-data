#include "tablewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProgramSkillWidget w;
    w.show();

    return a.exec();
}
