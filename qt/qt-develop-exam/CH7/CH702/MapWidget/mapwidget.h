#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QGraphicsView>
#include <QLabel>
#include <QMouseEvent>

class MapWidget : public QGraphicsView
{
    Q_OBJECT
public:
    MapWidget();

    void readMap();                         //读取地图信息
    QPointF mapToMap(QPointF);  			//用于实现场景坐标系与地图坐标之间的映射，以获得某点的经纬度值

public slots:
    void slotZoom(int);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect); 		//完成地图显示的功能
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPixmap map;
    qreal zoom;
    QLabel *viewCoord;
    QLabel *sceneCoord;
    QLabel *mapCoord;
    double x1,y1;
    double x2,y2;
};

#endif // MAPWIDGET_H
