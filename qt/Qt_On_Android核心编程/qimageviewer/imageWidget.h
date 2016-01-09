#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include "imageLoadManager.h"
#include "../openFileWidget.h"
#include "../imageButton.h"

class QGestureEvent;
class QPanGesture;
class QPinchGesture;
class QSwipeGesture;

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

protected:
    bool event(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

protected slots:
    void onFileSelected(QString strFile);
    void onFileSelectCanceled();
    void goNextImage();
    void goPrevImage();

private:
    bool gestureEvent(QGestureEvent *event);
    void panTriggered(QPanGesture*);
    void pinchTriggered(QPinchGesture*);
    void swipeTriggered(QSwipeGesture*);
    void resetPaintParameters();

    void openDirectory();
    void updateImage();
    void loadImage();
    void goToImage(int index);
    void reposControllers(const QSize &size);
    void checkNextPrevIndicator();

    QString m_strPath;
    QStringList m_files;
    int m_position;
    QImage m_image;
    ImageLoadManager *m_loader;
    OpenFileWidget * m_fileSelector;
    QLabel *m_loadingLabel;
    ImageButton * m_prevButton;
    ImageButton * m_nextButton;

    float m_horizontalOffset;
    float m_verticalOffset;
    float m_rotationAngle;
    float m_scaleFactor;
    float m_currentStepScaleFactor;
    bool m_bLoading;
};

#endif
