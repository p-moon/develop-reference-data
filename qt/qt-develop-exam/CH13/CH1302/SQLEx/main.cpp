#include "mainwindow.h"
#include <QApplication>
#include <QDialog>
#include <QFile>
#include "connectdlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    ConnDlg dialog;
    if (dialog.exec() != QDialog::Accepted)
        return -1;
    //dialog.show();
    QFile *carDetails = new QFile("attribs.xml");
    MainWindow window("factory", "cars", carDetails);
    window.show();
    
    return a.exec();
}
