#include "svgwindow.h"

SvgWindow::SvgWindow(QWidget *parent)
    :QScrollArea(parent)
{
    svgWidget =new SvgWidget;
    setWidget(svgWidget);
}

void SvgWindow::setFile(QString fileName)
{
    svgWidget->load(fileName);
    QSvgRenderer *render =svgWidget->renderer();
    svgWidget->resize(render->defaultSize());
}

void SvgWindow::mousePressEvent(QMouseEvent *event)
{
    mousePressPos =event->pos();
    scrollBarValuesOnMousePress.rx()=horizontalScrollBar()->value();
    scrollBarValuesOnMousePress.ry()=verticalScrollBar()->value();
    event->accept();
}

void SvgWindow::mouseMoveEvent(QMouseEvent *event)
{
    horizontalScrollBar()->setValue(scrollBarValuesOnMousePress.x()-event->pos().x()+mousePressPos.x());	//对水平滑动条的新位置进行设置
    verticalScrollBar()->setValue(scrollBarValuesOnMousePress.y()-event->pos().y()+mousePressPos.y());      //对垂直滑动条的新位置进行设置
    horizontalScrollBar()->update();
    verticalScrollBar()->update();
    event->accept();
}
