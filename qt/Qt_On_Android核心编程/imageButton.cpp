#include "imageButton.h"
#include <QPainter>
#include <QRect>

ImageButton::ImageButton(QPixmap normal, QPixmap pressed, QWidget *parent)
    : QAbstractButton(parent)
    , m_normal(normal)
    , m_pressed(pressed)
    , m_bPressed(false)
{
    connect(this, SIGNAL(pressed()), this, SLOT(onPressed()));
    connect(this, SIGNAL(released()), this, SLOT(onReleased()));
    setAttribute(Qt::WA_NoSystemBackground);
}

void ImageButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    if(m_bPressed)
    {
        painter.drawPixmap(rect(), m_pressed);
    }
    else
    {
        painter.drawPixmap(rect(), m_normal);
    }
}

void ImageButton::onPressed()
{
    m_bPressed = true;
    update();
}

void ImageButton::onReleased()
{
    m_bPressed = false;
    update();
}
