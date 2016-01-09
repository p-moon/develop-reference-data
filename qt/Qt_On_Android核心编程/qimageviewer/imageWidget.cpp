#include "imageWidget.h"
#include <QEvent>
#include <QGesture>
#include <QGestureEvent>
#include <QPainter>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent),
    m_position(0),
    m_horizontalOffset(0),
    m_verticalOffset(0),
    m_rotationAngle(0),
    m_scaleFactor(1),
    m_currentStepScaleFactor(1),
    m_bLoading(false)
{
    setAttribute(Qt::WA_AcceptTouchEvents, true);
    setMinimumSize(QSize(320,240));

    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::SwipeGesture);

    m_loader = new ImageLoadManager(this, this);
    connect(m_loader, SIGNAL(finished()), m_loader, SLOT(deleteLater()));
    m_loader->start();
    m_loadingLabel = new QLabel("正在努力为您加载...", this);
    m_loadingLabel->setVisible(false);
    m_loadingLabel->adjustSize();

    QSize iconSize(64, 64);
    m_prevButton = new ImageButton(QPixmap(":/images/prev.png"),
                                   QPixmap(":/images/prev_pressed.png"),
                                   this);
    m_prevButton->setFixedSize(iconSize);
    //m_prevButton->setStyleSheet("*{background-color: transparent;}");
    m_prevButton->setContentsMargins(0, 0, 0, 0);
    connect(m_prevButton, SIGNAL(clicked()), this, SLOT(goPrevImage()));
    m_prevButton->setVisible(false);
    m_prevButton->adjustSize();

    m_nextButton = new ImageButton(QPixmap(":/images/next.png"),
                                   QPixmap(":/images/next_pressed.png"),
                                   this);
    m_nextButton->setFixedSize(iconSize);
    //m_nextButton->setStyleSheet("*{background-color: transparent;}");
    m_nextButton->setContentsMargins(0, 0, 0, 0);
    connect(m_nextButton, SIGNAL(clicked()), this, SLOT(goNextImage()));
    m_nextButton->setVisible(false);
    m_nextButton->adjustSize();

    openDirectory();
}

ImageWidget::~ImageWidget()
{
    m_loader->quit();
    m_loader->wait(1000);
}

bool ImageWidget::event(QEvent *event)
{
    QEvent::Type t = event->type();
    switch(t)
    {
    case QEvent::Gesture:
        return gestureEvent(static_cast<QGestureEvent*>(event));
    default:
        break;
    }
    if( t == ImageLoadedEvent::evType())
    {
        //got image
        ImageLoadedEvent *ile = (ImageLoadedEvent*)event;
        if(ile->loaded() && ile->magicNum() == m_position)
        {
            resetPaintParameters();
            m_image = ile->image();
            update();
        }

        m_bLoading = m_loader->pendingImageLoadElement() > 0;
        if(!m_bLoading)
        {
            m_loadingLabel->setVisible(false);
        }
        return true;
    }
    return QWidget::event(event);
}

void ImageWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    if(!m_image.isNull())
    {
        float iw = m_image.width();
        float ih = m_image.height();
        float wh = height();
        float ww = width();

        p.translate(ww/2, wh/2);
        p.translate(m_horizontalOffset, m_verticalOffset);
        //p.rotate(m_rotationAngle);
        p.scale(m_currentStepScaleFactor * m_scaleFactor, m_currentStepScaleFactor * m_scaleFactor);
        p.translate(-iw/2, -ih/2);
        p.drawImage(0, 0, m_image);
    }
}

void ImageWidget::onFileSelected(QString strFile)
{
    m_fileSelector->getPath(m_strPath);
    m_fileSelector->getFileList(m_files);
    m_fileSelector = 0;
    goToImage(m_files.indexOf(strFile));
    update();
    checkNextPrevIndicator();
}

void ImageWidget::onFileSelectCanceled()
{
    m_fileSelector = 0;
}

//gesture event handler
bool ImageWidget::gestureEvent(QGestureEvent *event)
{
    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    else if (QGesture *pan = event->gesture(Qt::PanGesture))
        panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void ImageWidget::panTriggered(QPanGesture *gesture)
{
    QPointF delta = gesture->delta();
#ifdef USE_PAN_CHANGE_IMAGE
    int xDistance = delta.x();
    int yDistance = delta.y();
    if(xDistance >= 50 || yDistance >= 50)
    {
        // move to right or down
        goPrevImage();
    }
    else if(xDistance <= -50 || yDistance <= -50)
    {
        //move to left or up
        goNextImage();
    }
#else
    m_horizontalOffset += delta.x();
    m_verticalOffset += delta.y();
    update();
#endif
}

void ImageWidget::pinchTriggered(QPinchGesture *gesture)
{
    QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
    /*
    if (changeFlags & QPinchGesture::RotationAngleChanged)
    {
        qreal value = gesture->property("rotationAngle").toReal();
        qreal lastValue = gesture->property("lastRotationAngle").toReal();
        m_rotationAngle += value - lastValue;
    }
    */
    if (changeFlags & QPinchGesture::ScaleFactorChanged)
    {
        qreal value = gesture->scaleFactor();
        m_currentStepScaleFactor *= value;
    }
    if (gesture->state() == Qt::GestureFinished)
    {
        m_scaleFactor *= m_currentStepScaleFactor;
        m_currentStepScaleFactor = 1;
    }
    update();
}

void ImageWidget::swipeTriggered(QSwipeGesture *gesture)
{
    qDebug() << "swipeTriggered";
    if (gesture->state() == Qt::GestureFinished) {
        if (gesture->horizontalDirection() == QSwipeGesture::Left
            || gesture->verticalDirection() == QSwipeGesture::Up)
            goPrevImage();
        else
            goNextImage();
        update();
    }
}

void ImageWidget::resetPaintParameters()
{
    m_horizontalOffset = 0;
    m_verticalOffset = 0;
    m_rotationAngle = 0;
    m_scaleFactor = 1;
    m_currentStepScaleFactor = 1;
}

void ImageWidget::resizeEvent(QResizeEvent* e)
{
    const QSize &size = e->size();
    if(m_fileSelector) m_fileSelector->setFixedSize(size);
    reposControllers(size);
    update();
}

void ImageWidget::openDirectory()
{
    m_strPath = QDir::currentPath();
    QStringList nameFilters("*.jpg");
    nameFilters << "*.png" << "*.bmp" << "*.gif";
    m_fileSelector = new OpenFileWidget(m_strPath, nameFilters, this);
    m_fileSelector->setAttribute(Qt::WA_DeleteOnClose);
    connect(m_fileSelector, SIGNAL(selected(QString))
            , this, SLOT(onFileSelected(QString)));
    connect(m_fileSelector, SIGNAL(canceled())
            , this, SLOT(onFileSelectCanceled()));
}

void ImageWidget::loadImage()
{
    if(m_bLoading)
    {
        m_loader->abortLoading();
    }
    else
    {
        m_bLoading = true;
        m_loadingLabel->setVisible(true);
        update();
    }
    QString path = QString("%1/%2").arg(m_strPath)
            .arg(m_files.at(m_position));
    m_loader->addImageLoadElement(path, 0, QSize(), m_position);
}

void ImageWidget::goNextImage()
{
    if (m_files.isEmpty())
        return;

    if (m_position < m_files.size()-1)
    {
        ++m_position;
        loadImage();
        checkNextPrevIndicator();
    }
}

void ImageWidget::goPrevImage()
{
    if (m_files.isEmpty())
        return;

    if (m_position > 0) {
        --m_position;
        loadImage();
        checkNextPrevIndicator();
    }
}

void ImageWidget::goToImage(int index)
{
    if (m_files.isEmpty())
        return;

    if (index < 0 || index >= m_files.size()) {
        qDebug() << "goToImage: invalid index: " << index;
        return;
    }

    m_position = index;
    loadImage();
}

void ImageWidget::reposControllers(const QSize &size)
{
    int centerY = size.height() >> 1;
    int buttonY = centerY - (m_prevButton->height() >> 1);
    QSize loadSize = m_loadingLabel->size();
    m_loadingLabel->move((size.width() - loadSize.width())>>1
                         ,centerY - (loadSize.height()>>1));
    m_prevButton->move(2, buttonY);
    m_nextButton->move(size.width() - m_nextButton->width() -2, buttonY);
}

void ImageWidget::checkNextPrevIndicator()
{
    m_prevButton->setVisible(m_position > 0);
    m_nextButton->setVisible(m_position < m_files.count() - 1);
}
