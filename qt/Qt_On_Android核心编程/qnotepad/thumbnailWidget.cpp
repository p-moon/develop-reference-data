#include "thumbnailWidget.h"
#include <QPainter>

ThumbnailWidget::ThumbnailWidget(QNote *note, int w, int h, QPixmap *background)
    : m_note(note), m_background(background)
{
    setAutoFillBackground(false);
    setFixedSize(w,h);
}

void ThumbnailWidget::paintEvent(QPaintEvent * e)
{
    QPainter painter(this);

}
