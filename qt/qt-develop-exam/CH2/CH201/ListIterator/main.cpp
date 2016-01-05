#include <QDebug>
int main(int argc,char *argv[])
{
    QList<int> list;
    list<<1<<2<<3<<4<<5;
    QListIterator<int> i(list);
    for(;i.hasNext();)
        qDebug()<<i.next();
    return 0;
}

