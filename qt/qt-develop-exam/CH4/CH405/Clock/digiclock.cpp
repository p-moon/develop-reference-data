#include "digiclock.h"
#include <QTimer>
#include <QTime>
#include <QMouseEvent>

DigiClock::DigiClock(QWidget *parent)
    :QLCDNumber(parent)
{
    QPalette p=palette();
    p.setColor(QPalette::Window,Qt::blue);
    setPalette(p);

    setWindowFlags(Qt::FramelessWindowHint);

    setWindowOpacity(0.5);

    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start(1000);
    showTime();
    resize(150,60);
    showColon=true;                                 //初始化
}

void DigiClock::showTime()
{
    QTime time=QTime::currentTime();
    QString text=time.toString("hh:mm");
    if(showColon)
    {
        text[2]=':';
        showColon=false;
    }
    else
    {
        text[2]=' ';
        showColon=true;
    }
    display(text);
}

void DigiClock::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        dragPosition=event->globalPos()-frameGeometry().topLeft();
        event->accept();
    }
    if(event->button()==Qt::RightButton)
    {
        close();
    }
}
void DigiClock::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        move(event->globalPos()-dragPosition);
        event->accept();
    }
}

