#include <QDebug>
int main(int argc,char *argv[])
{
    QMap<QString,QString> map;
    map.insert("beijing","111");
    map.insert("shanghai","021");
    map.insert("nanjing","025");
    QMap<QString,QString>::const_iterator i;
    for(i=map.constBegin();i!=map.constEnd();++i)
        qDebug()<<"  "<<i.key()<<"  "<<i.value();
    QMap<QString,QString>::iterator mi;
    mi=map.find("beijing");
    if(mi!=map.end())
        mi.value()="010";
    QMap<QString,QString>::const_iterator modi;
    qDebug()<<"  ";
    for(modi=map.constBegin();modi!=map.constEnd();++modi)
        qDebug()<<"  "<<modi.key()<<"  "<<modi.value();
    return 0;
}

