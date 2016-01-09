#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QString>
#include <QFutureWatcher>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *e);

protected slots:
    void onLoad();
    void onFinished();

private:
    QFutureWatcher<QImage> m_watcher;
    QImage m_image;
};

#endif // WIDGET_H
