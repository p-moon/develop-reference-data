#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QMainWindow>
#include <QLabel>
#include <QStatusBar>
#include <QMouseEvent>

class MouseEvent : public QMainWindow
{
    Q_OBJECT
    
public:
    MouseEvent(QWidget *parent = 0);
    ~MouseEvent();
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
private:
    QLabel *statusLabel;
    QLabel *MousePosLabel;
};

#endif // MOUSEEVENT_H
