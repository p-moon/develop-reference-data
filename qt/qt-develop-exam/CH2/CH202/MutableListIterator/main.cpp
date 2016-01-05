#include <QDebug>
int main(int argc,char *argv[])
{
    QList<int> list;
    QMutableListIterator<int> i(list);
    for(int j=0;j<10;++j)
        i.insert(j);
    for(i.toFront();i.hasNext();)
        qDebug()<<i.next();
    for(i.toBack();i.hasPrevious();)
    {
        if(i.previous()%2==0)
            i.remove();
        else
            i.setValue(i.peekNext()*10);
    }
    for(i.toFront();i.hasNext();)
        qDebug()<<i.next();
    return 0;
}
