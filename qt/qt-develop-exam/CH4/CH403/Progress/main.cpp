#include "progressdlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProgressDlg w;
    w.show();
    
    return a.exec();
}
