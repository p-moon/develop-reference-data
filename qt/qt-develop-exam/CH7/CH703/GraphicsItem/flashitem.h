#ifndef FLASHITEM_H
#define FLASHITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

class FlashItem : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit FlashItem(QObject *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void timerEvent(QTimerEvent *);
private:
    bool flash;
    QTimer *timer;
signals:
    
public slots:
    
};

#endif // FLASHITEM_H
