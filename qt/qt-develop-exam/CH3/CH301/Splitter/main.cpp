#include "mainwindow.h"
#include <QApplication>
#include<Qsplitter>
#include<QTextEdit>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font("ZYSong18030",12);
    a.setFont(font);
    //主分割窗口
    QSplitter *splitterMain =new QSplitter(Qt::Horizontal,0);
    QTextEdit *textLeft =new QTextEdit(QObject::tr("Left Widget"),splitterMain);
    textLeft->setAlignment(Qt::AlignCenter);
    //右部分割窗口
    QSplitter *splitterRight =new QSplitter(Qt::Vertical,splitterMain);
    splitterRight->setOpaqueResize(false);
    QTextEdit *textUp =new QTextEdit(QObject::tr("Top Widget"),splitterRight);
    textUp->setAlignment(Qt::AlignCenter);
    QTextEdit *textBottom =new QTextEdit(QObject::tr("Bottom Widget"),splitterRight);
    textBottom->setAlignment(Qt::AlignCenter);
    splitterMain->setStretchFactor(1,1);
    splitterMain->setWindowTitle(QObject::tr("Splitter"));
    splitterMain->show();

    //MainWindow w;
    //w.show();
    
    return a.exec();
}
