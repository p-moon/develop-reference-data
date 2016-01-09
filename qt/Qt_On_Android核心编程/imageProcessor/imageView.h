#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QImage>
#include <QFutureWatcher>
#include <QBrush>

class ImageView : public QWidget
{
    Q_OBJECT
public:
    ImageView(QWidget *parent = 0);
    ~ImageView();

    void setSource(QString filePath);
    QString source() {return m_source;}

    void showBusyIndicator();

signals:
    void loaded(bool success);

protected:
    void paintEvent(QPaintEvent *e);
    void timerEvent(QTimerEvent *e);
    void resizeEvent(QResizeEvent *e);

protected slots:
    void onFinished();

protected:
    QImage m_image;
    QString m_source;
    QFutureWatcher<QImage> m_watcher;
    bool m_bBusy;
    int m_busyTimer;
    QRect m_busyRects[6];
    int m_currentRect;
    QBrush m_busyNormal;
    QBrush m_busyRunning;
};

#endif // IMAGEVIEW_H
