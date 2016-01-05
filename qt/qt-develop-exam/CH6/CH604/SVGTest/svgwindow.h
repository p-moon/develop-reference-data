#ifndef SVGWINDOW_H
#define SVGWINDOW_H

#include <QScrollArea>
#include "svgwidget.h"

class SvgWindow : public QScrollArea
{
    Q_OBJECT
public:
    SvgWindow(QWidget *parent=0);
    void setFile(QString);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
private:
    SvgWidget *svgWidget;
    QPoint mousePressPos;
    QPoint scrollBarValuesOnMousePress;
};

#endif // SVGWINDOW_H
