#include "eventfilter.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QMatrix>

EventFilter::EventFilter(QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    setWindowTitle(tr("事件过滤"));

    label1 = new QLabel;
    Image1.load("../image/1.png");
    label1->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    label1->setPixmap(QPixmap::fromImage(Image1));

    label2 = new QLabel;
    Image2.load("../image/2.png");
    label2->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    label2->setPixmap(QPixmap::fromImage(Image2));

    label3 = new QLabel;
    Image3.load("../image/3.png");
    label3->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    label3->setPixmap(QPixmap::fromImage(Image3));

    stateLabel = new QLabel(tr("鼠标按下标志"));
    stateLabel->setAlignment(Qt::AlignHCenter);
    QHBoxLayout *layout=new QHBoxLayout;
    layout->addWidget(label1);
    layout->addWidget(label2);
    layout->addWidget(label3);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->addWidget(stateLabel);

    label1->installEventFilter(this);
    label2->installEventFilter(this);
    label3->installEventFilter(this);
}

EventFilter::~EventFilter()
{
    
}

bool EventFilter::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==label1)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent=(QMouseEvent *)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
                stateLabel->setText(tr("左键按下左边图片"));
            }
            else if(mouseEvent->buttons()&Qt::MidButton)
            {
                stateLabel->setText(tr("中键按下左边图片"));
            }
            else if(mouseEvent->buttons()&Qt::RightButton)
            {
                stateLabel->setText(tr("右键按下左边图片"));
            }

            QMatrix matrix;
            matrix.scale(1.8,1.8);
            QImage tmpImg=Image1.transformed(matrix);
            label1->setPixmap(QPixmap::fromImage(tmpImg));
        }
        if(event->type()==QEvent::MouseButtonRelease)
        {
            stateLabel->setText(tr("鼠标释放左边图片"));
            label1->setPixmap(QPixmap::fromImage(Image1));
        }
    }
    else if(watched==label2)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent=(QMouseEvent *)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
                stateLabel->setText(tr("左键按下中间图片"));
            }
            else if(mouseEvent->buttons()&Qt::MidButton)
            {
                stateLabel->setText(tr("中键按下中间图片"));
            }
            else if(mouseEvent->buttons()&Qt::RightButton)
            {
                stateLabel->setText(tr("右键按下中间图片"));
            }

            QMatrix matrix;
            matrix.scale(1.8,1.8);
            QImage tmpImg=Image2.transformed(matrix);
            label2->setPixmap(QPixmap::fromImage(tmpImg));
        }
        if(event->type()==QEvent::MouseButtonRelease)
        {
            stateLabel->setText(tr("鼠标释放中间图片"));
            label2->setPixmap(QPixmap::fromImage(Image2));
        }
    }
    else if(watched==label3)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent=(QMouseEvent *)event;
            if(mouseEvent->buttons()&Qt::LeftButton)
            {
                stateLabel->setText(tr("左键按下右边图片"));
            }
            else if(mouseEvent->buttons()&Qt::MidButton)
            {
                stateLabel->setText(tr("中键按下右边图片"));
            }
            else if(mouseEvent->buttons()&Qt::RightButton)
            {
                stateLabel->setText(tr("右键按下右边图片"));
            }

            QMatrix matrix;
            matrix.scale(1.8,1.8);
            QImage tmpImg=Image3.transformed(matrix);
            label3->setPixmap(QPixmap::fromImage(tmpImg));
        }
        if(event->type()==QEvent::MouseButtonRelease)
        {
            stateLabel->setText(tr("鼠标释放右边图片"));
            label3->setPixmap(QPixmap::fromImage(Image3));
        }
    }
    return QDialog::eventFilter(watched,event);
}
