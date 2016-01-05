#include <QDebug>
int main(int argc,char *argv[])
{
    QMap<QString,QString> map;
    map.insert("beijing","111");
    map.insert("shanghai","021");
    map.insert("nanjing","025");
    QMapIterator<QString,QString> i(map);
    for(;i.hasNext();)
        qDebug()<<"  "<<i.key()<<"  "<<i.next().value();
    QMutableMapIterator<QString,QString> mi(map);
    if(mi.findNext("111"))
        mi.setValue("010");
    QMapIterator<QString,QString> modi(map);
    qDebug()<<"  ";
    for(;modi.hasNext();)
        qDebug()<<" "<<modi.key()<<"  "<<modi.next().value();
    return 0;
}


