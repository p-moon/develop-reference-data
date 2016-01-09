#include "widget.h"
#include <QEvent>
#include <QTouchEvent>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents);
}

Widget::~Widget()
{

}

bool Widget::event(QEvent *e)
{
    QEvent::Type type = e->type();
    switch(type)
    {
    case QEvent::TouchBegin:
        e->accept();
        dumpTouchEvent(e);
        return true;
    case QEvent::TouchUpdate:
        e->accept();
        dumpTouchEvent(e);
        return true;
    case QEvent::TouchEnd:
        e->accept();
        dumpTouchEvent(e);
        return true;
    case QEvent::TouchCancel:
        e->accept();
        dumpTouchEvent(e);
        return true;
    default:
        break;
    }
    return QWidget::event(e);
}

void Widget::dumpTouchEvent(QEvent *e)
{
    QTouchEvent * te = (QTouchEvent *)e;
    qDebug() << "type - " << te->type();
    qDebug() << "device - " << te->device();
    qDebug() << "touchPointStates - " << te->touchPointStates();
    const QList<QTouchEvent::TouchPoint> & points= te->touchPoints();
    int count = points.size();
    if(!count) return;
    for(int i = 0; i < count; i++)
    {
        const QTouchEvent::TouchPoint &point = points.at(i);
        qDebug() << "point " << i+1  << "information:";
        qDebug() << "  --relative to widget";
        qDebug() << "    pos - " << point.pos();
        qDebug() << "    startPos - " << point.startPos();
        qDebug() << "    lastPos - " << point.lastPos();
        qDebug() << "    rect - " << point.rect();
        qDebug() << "  --relative to screen";
        qDebug() << "    screenPos - " << point.screenPos();
        qDebug() << "    scrennRect - " << point.screenRect();
    }
}
