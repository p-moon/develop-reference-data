#include "butterfly.h"
#include <math.h>

const static double PI=3.1416;

Butterfly::Butterfly(QObject *parent) :
    QObject(parent)
{
    up = true;
    pix_up.load("up.png");
    pix_down.load("down.png");

    startTimer(100);
}

QRectF Butterfly::boundingRect() const
{
    qreal adjust =2;
    return QRectF(-pix_up.width()/2-adjust,-pix_up.height()/2-adjust,pix_up.width()+adjust*2,pix_up.height()+adjust*2);
}

void Butterfly::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(up)
    {
        painter->drawPixmap(boundingRect().topLeft(),pix_up);
        up=!up;
    }
    else
    {
        painter->drawPixmap(boundingRect().topLeft(),pix_down);
        up=!up;
    }
}

void Butterfly::timerEvent(QTimerEvent *)
{
    //边界控制
    qreal edgex=scene()->sceneRect().right()+boundingRect().width()/2;
    qreal edgetop=scene()->sceneRect().top()+boundingRect().height()/2;
    qreal edgebottom=scene()->sceneRect().bottom()+boundingRect(). height()/2;

    if(pos().x()>=edgex)
        setPos(scene()->sceneRect().left(),pos().y());
    if(pos().y()<=edgetop)
        setPos(pos().x(),scene()->sceneRect().bottom());
    if(pos().y()>=edgebottom)
        setPos(pos().x(),scene()->sceneRect().top());

    angle+=(qrand()%10)/20.0;
    qreal dx=fabs(sin(angle*PI)*10.0);
    qreal dy=(qrand()%20)-10.0;

    setPos(mapToParent(dx,dy));
}


