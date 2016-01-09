#include "widget.h"
#include "../simpleCustomEvent.h"
#include <QPainter>
#include <QImage>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include "../openFileWidget.h"
#include <QDir>
#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#endif
#include <QMessageBox>

extern QString g_tempPath;
extern QString g_sdPath;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_processor(this)
    , m_imageView(new ImageView)
    , m_lastOpenPath(g_sdPath)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    QHBoxLayout *topLayout = new QHBoxLayout();
    mainLayout->addLayout(topLayout);
    topLayout->addStretch(1);

    QPushButton *button = new QPushButton("拍照");
    connect(button, SIGNAL(clicked()), this, SLOT(onCameraButton()));
    topLayout->addWidget(button);
    m_controls.append(button);

    button = new QPushButton("文件");
    connect(button, SIGNAL(clicked()), this, SLOT(onOpenFileButton()));
    topLayout->addWidget(button);
    m_controls.append(button);

    button = new QPushButton("保存");
    connect(button, SIGNAL(clicked()), this, SLOT(onSaveButton()));
    topLayout->addWidget(button);
    m_controls.append(button);

    m_centralLayout = new QStackedLayout();
    mainLayout->addLayout(m_centralLayout, 1);
    m_centralLayout->addWidget(m_imageView);

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    mainLayout->addLayout(bottomLayout);

    button = new QPushButton("黑白");
    connect(button, SIGNAL(clicked()), this, SLOT(onBinarizeButton()));
    bottomLayout->addWidget(button);
    m_controls.append(button);

    button = new QPushButton("底片");
    connect(button, SIGNAL(clicked()), this, SLOT(onNegativeButton()));
    bottomLayout->addWidget(button);
    m_controls.append(button);

    button = new QPushButton("锐化");
    connect(button, SIGNAL(clicked()), this, SLOT(onSharpenButton()));
    bottomLayout->addWidget(button);
    m_controls.append(button);

    button = new QPushButton("模糊");
    connect(button, SIGNAL(clicked()), this, SLOT(onSoftenButton()));
    bottomLayout->addWidget(button);
    m_controls.append(button);

    button = new QPushButton("灰度");
    connect(button, SIGNAL(clicked()),
            this, SLOT(onGrayButton()));
    bottomLayout->addWidget(button);
    m_controls.append(button);

    button = new QPushButton("浮雕");
    connect(button, SIGNAL(clicked()), this, SLOT(onEmbossButton()));
    bottomLayout->addWidget(button);
    m_controls.append(button);

    bottomLayout->addStretch(1);

    connect(m_imageView, SIGNAL(loaded(bool)),
            this, SLOT(onImageLoaded(bool)));
    connect(&m_processor, SIGNAL(finished(QString)),
            this, SLOT(onImageProcessed(QString)));
    m_processor.setTempPath(g_tempPath);
}

Widget::~Widget()
{
}

bool Widget::event(QEvent *e)
{
    if(e->type() == SimpleCustomEvent::eventType())
    {
        e->accept();
        SimpleCustomEvent *sce = (SimpleCustomEvent*)e;
        if(sce->m_arg1 == 2)
        {
            m_origFilePath = sce->m_arg2;
            m_tempFilePath.clear();
            m_imageView->setSource(sce->m_arg2);
        }
        else
        {
            //TODO: show error
        }
        return true;
    }
    return QWidget::event(e);
}

void Widget::onBinarizeButton()
{
    setActionsEnabled(false);
    m_imageView->showBusyIndicator();
    m_processor.process(m_origFilePath,
                        ImageProcessor::Binarize);
}

void Widget::onNegativeButton()
{
    setActionsEnabled(false);
    m_imageView->showBusyIndicator();
    m_processor.process(m_origFilePath,
                        ImageProcessor::Negative);
}

void Widget::onSharpenButton()
{
    setActionsEnabled(false);
    m_imageView->showBusyIndicator();
    m_processor.process(m_origFilePath,
                        ImageProcessor::Sharpen);
}

void Widget::onSoftenButton()
{
    setActionsEnabled(false);
    m_imageView->showBusyIndicator();
    m_processor.process(m_origFilePath,
                        ImageProcessor::Soften);
}

void Widget::onGrayButton()
{
    setActionsEnabled(false);
    m_imageView->showBusyIndicator();
    m_processor.process(m_origFilePath,
                        ImageProcessor::Gray);
}

void Widget::onEmbossButton()
{
    setActionsEnabled(false);
    m_imageView->showBusyIndicator();
    m_processor.process(m_origFilePath,
                        ImageProcessor::Emboss);
}

void Widget::onOpenFileButton()
{
    if(m_lastOpenPath.isEmpty()) m_lastOpenPath = QDir::currentPath();
    QStringList nameFilters("*.jpg");
    nameFilters << "*.png" << "*.bmp" << "*.gif"
                << "*.JPG" << "*.PNG";
    OpenFileWidget *w = new OpenFileWidget(m_lastOpenPath, nameFilters);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->setMaximumSize(size());
    connect(w, SIGNAL(selected(QString))
            , this, SLOT(onFileSelected(QString)));
    connect(w, SIGNAL(canceled())
            , this, SLOT(onFileSelectCanceled()));
    int index = m_centralLayout->addWidget(w);
    m_centralLayout->setCurrentIndex(index);

    setActionsVisible(false);
}

void Widget::onCameraButton()
{
#ifdef Q_OS_ANDROID
    QAndroidJniObject::callStaticMethod<void>(
                "an/qt/imageProcessor/ImageProcessorActivity",
                "captureImage");
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck())
    {
        qDebug() << "onCaptureImage, got exception";
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
#endif
}

void Widget::onSaveButton()
{
    if( QMessageBox::Yes ==
            QMessageBox::question(this, "应用更改",
                                  "将覆盖原有图片，是否继续？"))
    {
        bool ok = (QFile::remove(m_origFilePath) &&
                   QFile::copy(m_tempFilePath, m_origFilePath));
        QMessageBox::information(this, "提示",
                                 ok ? "应用成功！":"应用失败");
    }
}

void Widget::onFileSelected(QString strFile)
{
    OpenFileWidget *w = qobject_cast<OpenFileWidget*>(sender());
    QString path;
    w->getPath(path);
    m_lastOpenPath = path;
    m_centralLayout->removeWidget(w);
    m_centralLayout->setCurrentWidget(m_imageView);

    if(path.endsWith('/')) path += strFile;
    else path = QString("%1/%2").arg(path).arg(strFile);
    m_origFilePath = path;
    m_tempFilePath.clear();
    m_imageView->setSource(path);

    setActionsVisible(true);
}

void Widget::onFileSelectCanceled()
{
    OpenFileWidget *w = qobject_cast<OpenFileWidget*>(sender());
    m_centralLayout->removeWidget(w);
    m_centralLayout->setCurrentWidget(m_imageView);

    setActionsVisible(true);
}

void Widget::onImageProcessed(QString newFile)
{
    m_tempFilePath = newFile;
    m_imageView->setSource(newFile);
}

void Widget::onImageLoaded(bool bSuccess)
{
    setActionsEnabled(true);
}

void Widget::setActionsEnabled(bool enable)
{
    foreach (QWidget * w, m_controls) {
        w->setEnabled(enable);
    }
}

void Widget::setActionsVisible(bool visible)
{
    foreach (QWidget * w, m_controls) {
        w->setVisible(visible);
    }
}

