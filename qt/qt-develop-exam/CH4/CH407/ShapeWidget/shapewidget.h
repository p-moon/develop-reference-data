#ifndef SHAPEWIDGET_H
#define SHAPEWIDGET_H

#include <QWidget>

class ShapeWidget : public QWidget
{
    Q_OBJECT
    
public:
    ShapeWidget(QWidget *parent = 0);
    ~ShapeWidget();
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
private:
    QPoint dragPosition;
};

#endif // SHAPEWIDGET_H
