#include "widget.h"
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include <QPainter>
#include <QTimer>

QImage loadImage(const QString & uri)
{
    QImage image(uri);
    return image;
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_watcher(this)
{
    connect(&m_watcher, SIGNAL(finished()),
            this, SLOT(onFinished()));
    QTimer::singleShot(5000, this, SLOT(onLoad()));
}

void Widget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(m_image.isNull())
    {
        painter.drawText(rect(), Qt::AlignCenter, "Loading...");
    }
    else
    {
        painter.drawImage(rect(), m_image);
    }
}

void Widget::onLoad()
{
    QString uri(":/airplay.gif");
    QFuture<QImage> future =
            QtConcurrent::run(loadImage, uri);
    m_watcher.setFuture(future);
}

void Widget::onFinished()
{
    m_image = m_watcher.future().result();
    if(!m_image.isNull())
    {
        repaint();
    }
}

