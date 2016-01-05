#include <QDebug>
int main(int argc,char *argv[])
{
    double a=-19.3,b=9.7;
    double c=qAbs(a);        //c=19.3
    double max=qMax(b,c);    //max=c=19.3

    int bn=qRound(b);        //bn=10
    int cn=qRound(c);        //cn=19

    qDebug()<<"a="<<a;
    qDebug()<<"b="<<b;
    qDebug()<<"c=qAbs(a)= "<<c;
    qDebug()<<"qMax(b,c)= "<<max;
    qDebug()<<"bn=qRound(b)= "<<bn;
    qDebug()<<"cn=qRound(c)= "<<cn;

    qSwap(bn,cn);
    qDebug()<<"qSwap(bn,cn)："<<"bn="<<bn<<" cn="<<cn;

    return 0;
}

