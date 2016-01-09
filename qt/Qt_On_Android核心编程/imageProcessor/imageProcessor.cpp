#include "imageProcessor.h"
#include <QThreadPool>
#include <QList>
#include <QFile>
#include <QFileInfo>
#include <QRunnable>
#include <QEvent>
#include <QCoreApplication>
#include <QPointer>
#include <QUrl>
#include <QImage>
#include <QDebug>
#include <QDir>

typedef void (*AlgorithmFunction)(QString sourceFile, QString destFile);

class AlgorithmRunnable;
class ExcutedEvent : public QEvent
{
public:
    ExcutedEvent(AlgorithmRunnable *r)
        : QEvent(evType()), m_runnable(r)
    {

    }
    AlgorithmRunnable *m_runnable;

    static QEvent::Type evType()
    {
        if(s_evType == QEvent::None)
        {
            s_evType = (QEvent::Type)registerEventType();
        }
        return s_evType;
    }

private:
    static QEvent::Type s_evType;
};
QEvent::Type ExcutedEvent::s_evType = QEvent::None;

static void _gray(QString sourceFile, QString destFile)
{
    QImage image(sourceFile);
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    qDebug() << "depth - " << image.depth();

    int width = image.width();
    int height = image.height();
    QRgb color;
    int gray;
    for(int i = 0; i < width; i++)
    {
        for(int j= 0; j < height; j++)
        {
            color = image.pixel(i, j);
            gray = qGray(color);
            image.setPixel(i, j, qRgba(gray, gray, gray, qAlpha(color)));
        }
    }

    image.save(destFile);
}

static void _binarize(QString sourceFile, QString destFile)
{
    QImage image(sourceFile);
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    int width = image.width();
    int height = image.height();
    QRgb color;
    QRgb avg;
    QRgb black = qRgb(0, 0, 0);
    QRgb white = qRgb(255, 255, 255);
    for(int i = 0; i < width; i++)
    {
        for(int j= 0; j < height; j++)
        {
            color = image.pixel(i, j);
            avg = (qRed(color) + qGreen(color) + qBlue(color))/3;
            image.setPixel(i, j, avg >= 128 ? white : black);
        }
    }
    image.save(destFile);
}

static void _negative(QString sourceFile, QString destFile)
{
    QImage image(sourceFile);
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    int width = image.width();
    int height = image.height();
    QRgb color;
    QRgb negative;
    for(int i = 0; i < width; i++)
    {
        for(int j= 0; j < height; j++)
        {
            color = image.pixel(i, j);
            negative = qRgba(255 - qRed(color),
                             255 - qGreen(color),
                             255 - qBlue(color),
                             qAlpha(color));
            image.setPixel(i, j, negative);
        }
    }
    image.save(destFile);
}

static void _emboss(QString sourceFile, QString destFile)
{
    QImage image(sourceFile);
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    int width = image.width();
    int height = image.height();
    QRgb color;
    QRgb preColor = 0;
    QRgb newColor;
    int gray, r, g, b, a;
    for(int i = 0; i < width; i++)
    {
        for(int j= 0; j < height; j++)
        {
            color = image.pixel(i, j);
            r = qRed(color) - qRed(preColor) + 128;
            g = qGreen(color) - qGreen(preColor) + 128;
            b = qBlue(color) - qBlue(preColor) + 128;
            a = qAlpha(color);
            gray = qGray(r, g, b);
            newColor = qRgba(gray, gray, gray, a);
            image.setPixel(i, j, newColor);
            preColor = newColor;
        }
    }
    image.save(destFile);
}

static void _sharpen(QString sourceFile, QString destFile)
{
    QImage image(sourceFile);
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    int width = image.width();
    int height = image.height();
    int threshold = 80;
    QImage sharpen(width, height, QImage::Format_ARGB32);
    int r, g, b, gradientR, gradientG, gradientB;
    QRgb rgb00, rgb01, rgb10;
    for(int i = 0; i < width; i++)
    {
        for(int j= 0; j < height; j++)
        {
            if(image.valid(i, j) &&
                    image.valid(i+1, j) &&
                    image.valid(i, j+1))
            {
                rgb00 = image.pixel(i, j);
                rgb01 = image.pixel(i, j+1);
                rgb10 = image.pixel(i+1, j);
                r = qRed(rgb00);
                g = qGreen(rgb00);
                b = qBlue(rgb00);
                gradientR = abs(r - qRed(rgb01)) + abs(r - qRed(rgb10));
                gradientG = abs(g - qGreen(rgb01)) + abs(g - qGreen(rgb10));
                gradientB = abs(b - qBlue(rgb01)) + abs(b - qBlue(rgb10));

                if(gradientR > threshold)
                {
                    r = qMin(gradientR + 100, 255);
                }

                if(gradientG > threshold)
                {
                    g = qMin( gradientG + 100, 255);
                }

                if(gradientB > threshold)
                {
                    b = qMin( gradientB + 100, 255);
                }

                sharpen.setPixel(i, j, qRgb(r, g, b));
            }
        }
    }

    sharpen.save(destFile);
}

static void _soften(QString sourceFile, QString destFile)
{
    QImage image(sourceFile);
    if(image.isNull())
    {
        qDebug() << "load " << sourceFile << " failed! ";
        return;
    }
    int width = image.width();
    int height = image.height();
    int r, g, b;
    QRgb color;
    int xLimit = width - 1;
    int yLimit = height - 1;
    for(int i = 1; i < xLimit; i++)
    {
        for(int j = 1; j < yLimit; j++)
        {
            r = 0;
            g = 0;
            b = 0;
            for(int m = 0; m < 9; m++)
            {
                int s = 0;
                int p = 0;
                switch(m)
                {
                case 0:
                    s = i - 1;
                    p = j - 1;
                    break;
                case 1:
                    s = i;
                    p = j - 1;
                    break;
                case 2:
                    s = i + 1;
                    p = j - 1;
                    break;
                case 3:
                    s = i + 1;
                    p = j;
                    break;
                case 4:
                    s = i + 1;
                    p = j + 1;
                    break;
                case 5:
                    s = i;
                    p = j + 1;
                    break;
                case 6:
                    s = i - 1;
                    p = j + 1;
                    break;
                case 7:
                    s = i - 1;
                    p = j;
                    break;
                case 8:
                    s = i;
                    p = j;
                }
                color = image.pixel(s, p);
                r += qRed(color);
                g += qGreen(color);
                b += qBlue(color);
            }

            r = (int) (r / 9.0);
            g = (int) (g / 9.0);
            b = (int) (b / 9.0);

            r = qMin(255, qMax(0, r));
            g = qMin(255, qMax(0, g));
            b = qMin(255, qMax(0, b));

            image.setPixel(i, j, qRgb(r, g, b));
        }
    }

    image.save(destFile);
}


static AlgorithmFunction g_functions[ImageProcessor::AlgorithmCount] = {
    _gray,
    _binarize,
    _negative,
    _emboss,
    _sharpen,
    _soften
};

class AlgorithmRunnable : public QRunnable
{
public:
    AlgorithmRunnable(
            QString sourceFile,
            QString destFile,
            ImageProcessor::ImageAlgorithm algorithm,
            QObject * observer)
        : m_observer(observer)
        , m_sourceFilePath(sourceFile)
        , m_destFilePath(destFile)
        , m_algorithm(algorithm)
    {
    }
    ~AlgorithmRunnable(){}


    void run()
    {
        qDebug() << "algorithm running...";
        g_functions[m_algorithm](m_sourceFilePath, m_destFilePath);
        QCoreApplication::postEvent(m_observer, new ExcutedEvent(this));
    }

    QPointer<QObject> m_observer;
    QString m_sourceFilePath;
    QString m_destFilePath;
    ImageProcessor::ImageAlgorithm m_algorithm;
};

class ImageProcessorPrivate : public QObject
{
public:
    ImageProcessorPrivate(ImageProcessor *processor)
        : QObject(processor), m_processor(processor),
          m_tempPath(QDir::currentPath())
    {
        ExcutedEvent::evType();
    }
    ~ImageProcessorPrivate()
    {

    }

    bool event(QEvent * e)
    {
        if(e->type() == ExcutedEvent::evType())
        {
            ExcutedEvent *ee = (ExcutedEvent*)e;
            if(m_runnables.contains(ee->m_runnable))
            {
                m_notifiedAlgorithm = ee->m_runnable->m_algorithm;
                m_notifiedSourceFile = ee->m_runnable->m_sourceFilePath;
                emit m_processor->finished(ee->m_runnable->m_destFilePath);
                m_runnables.removeOne(ee->m_runnable);
            }
            delete ee->m_runnable;
            return true;
        }
        return QObject::event(e);
    }

    void process(QString sourceFile, ImageProcessor::ImageAlgorithm algorithm)
    {
        QFileInfo fi(sourceFile);
        QString destFile = QString("%1/%2_%3").arg(m_tempPath)
                .arg((int)algorithm).arg(fi.fileName());
        AlgorithmRunnable *r = new AlgorithmRunnable(sourceFile,
                                                     destFile,
                                                     algorithm,
                                                     this);
        m_runnables.append(r);
        r->setAutoDelete(false);
        QThreadPool::globalInstance()->start(r);
    }

    ImageProcessor * m_processor;
    QList<AlgorithmRunnable*> m_runnables;
    QString m_notifiedSourceFile;
    ImageProcessor::ImageAlgorithm m_notifiedAlgorithm;
    QString m_tempPath;
};

ImageProcessor::ImageProcessor(QObject *parent)
    : QObject(parent)
    , m_d(new ImageProcessorPrivate(this))
{}
ImageProcessor::~ImageProcessor()
{
    delete m_d;
}

QString ImageProcessor::sourceFile() const
{
    return m_d->m_notifiedSourceFile;
}

ImageProcessor::ImageAlgorithm ImageProcessor::algorithm() const
{
    return m_d->m_notifiedAlgorithm;
}

void ImageProcessor::setTempPath(QString tempPath)
{
    m_d->m_tempPath = tempPath;
}

void ImageProcessor::process(QString file, ImageAlgorithm algorithm)
{
    qDebug() << "ImageProcessor::process - " << file << " algorithm " << algorithm;
    m_d->process(file, algorithm);
}

void ImageProcessor::abort(QString file, ImageAlgorithm algorithm)
{
    int size = m_d->m_runnables.size();
    AlgorithmRunnable *r;
    for(int i = 0; i < size; i++)
    {
        r = m_d->m_runnables.at(i);
        if(r->m_sourceFilePath == file && r->m_algorithm == algorithm)
        {
            m_d->m_runnables.removeAt(i);
            break;
        }
    }
}

void ImageProcessor::abortAll()
{
    m_d->m_runnables.clear();
}
