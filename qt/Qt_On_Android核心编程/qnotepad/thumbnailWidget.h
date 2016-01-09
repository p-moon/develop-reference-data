#ifndef THUMBNAILWIDGET_H
#define THUMBNAILWIDGET_H
#include <QWidget>
#include "note.h"

class ThumbnailWidget : public QWidget{
public:
    ThumbnailWidget(QNote *note, int w, int h, QPixmap *background);
protected:
    void paintEvent(QPaintEvent *);
private:
    QNote *m_note;
    QPixmap *m_background;
};
