#include "imgprocessor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont f("ZYSong18030",12);                        //设置显示的字体格式
    a.setFont(f);
    ImgProcessor w;
    w.show();
    
    return a.exec();
}
