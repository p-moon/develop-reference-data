#include "imageView.h"
#include <QPainter>
#include <QtConcurrent>
#include <QFuture>
#include <QResizeEvent>

QImage loadImage(const QString & uri, const QSize & size)
{
    QImage image(uri);
    return image;
}

ImageView::ImageView(QWidget *parent)
    : QWidget(parent)
    , m_watcher(this)
    , m_bBusy(false)
    , m_busyTimer(0)
    , m_currentRect(0)
    , m_busyNormal(Qt::darkGray)
    , m_busyRunning(Qt::darkBlue)
{
    connect(&m_watcher, SIGNAL(finished()),
            this, SLOT(onFinished()));
}

ImageView::~ImageView()
{

}

static QRect getShrinkOrEqualRect(const QRect &rcDest, const QRect &rcSrc)
{
    int destH = rcDest.height();
    int destW = rcDest.width();
    int srcW = rcSrc.width();
    int srcH = rcSrc.height();
    if(srcW <= destW && srcH <= destH)
    {
        int xOffset = (destW - srcW)/2;
        int yOffset = (destH - srcH)/2;

        return rcDest.adjusted(xOffset, yOffset, xOffset - (destW - srcW), yOffset - (destH - srcH));
    }

    QRect rc(rcDest);
    int h = (destW * srcH) / srcW;
    if(h < destH )
    {
        rc.setHeight(h);
        rc.moveTop( rc.top() + (destH - h)/2 );
    }
    else
    {
        int w = (destH * srcW) / srcH;
        rc.setWidth(w);
        rc.moveLeft( rc.left() + (destW - w)/2 );
    }
    return rc;
}

void ImageView::setSource(QString filePath)
{
    m_source = filePath;
    QFuture<QImage> future =
            QtConcurrent::run(loadImage, filePath, size());
    m_watcher.setFuture(future);
    showBusyIndicator();
}

void ImageView::showBusyIndicator()
{
    if(!m_bBusy)
    {
        m_bBusy = true;
        update();
    }
    if(m_busyTimer == 0) m_busyTimer = startTimer(500);
}

void ImageView::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    if(!m_image.isNull())
    {
        QRect dest = rect();
        QRect source = m_image.rect();
        dest = getShrinkOrEqualRect(dest, source);
        p.drawImage(dest, m_image, source);
    }
    if(m_bBusy)
    {
        p.setPen(Qt::NoPen);
        p.setBrush(m_busyRunning);
        p.drawRect(m_busyRects[m_currentRect]);
        p.setBrush(m_busyNormal);
        int i = 0;
        for(; i < m_currentRect; i++)
        {
            p.drawRect(m_busyRects[i]);
        }
        for(++i; i < 6; i++)
        {
            p.drawRect(m_busyRects[i]);
        }
    }
}

void ImageView::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == m_busyTimer)
    {
        e->accept();
        if(++m_currentRect >= 6) m_currentRect = 0;
        update();
        return;
    }
    QWidget::timerEvent(e);
}

void ImageView::resizeEvent(QResizeEvent *e)
{
    QSize size = e->size();
    int x = (size.width() - 4*5 - 16*6) >> 1;
    int y = (size.height() >> 1) - 8;
    for(int i = 0; i < 6; i++, x += 20)
    {
        m_busyRects[i].setRect(x, y, 16, 16);
    }
}

void ImageView::onFinished()
{
    m_bBusy = false;
    if(m_busyTimer > 0)
    {
        killTimer(m_busyTimer);
        m_busyTimer = 0;
        m_currentRect = 0;
    }

    m_image = m_watcher.future().result();
    if(m_image.isNull())
    {
        //TODO: show error
    }
    else
    {
        update();
    }
    emit loaded(!m_image.isNull());
}
