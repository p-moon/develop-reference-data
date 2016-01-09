#include "imageLoadManager.h"
#include <QCoreApplication>
#include <QFile>
#include <QDebug>

//
// class ImageLoadedEvent
//
QEvent::Type ImageLoadedEvent::s_evType = QEvent::None;
ImageLoadedEvent::ImageLoadedEvent(QImage image, void *userdata, QString filePath, bool bLoaded, uint iMagicNum)
    : QEvent(evType())
    , m_pUserData(userdata)
    , m_image(image)
    , m_bLoaded(bLoaded)
    , m_strFilePath(filePath)
    , m_iMagicNum(iMagicNum)
{
}

ImageLoadedEvent::~ImageLoadedEvent()
{
}

QEvent::Type ImageLoadedEvent::evType()
{
    if(s_evType == QEvent::None)
    {
        s_evType = (QEvent::Type)registerEventType();
    }

    return s_evType;
}

//
//class ImageLoadElement
//
ImageLoadElement::ImageLoadElement(const QString & filepath, void * userdata, QSize sizeForSave,uint iMagicNum)
    : m_strFile(filepath)
    , m_sizeForSave(sizeForSave)
    , m_pUserData(userdata)
    , m_iMagicNum(iMagicNum)
{
}

ImageLoadElement::ImageLoadElement(const ImageLoadElement &rhs)
    : m_strFile(rhs.m_strFile)
    , m_rawdata(rhs.m_rawdata)
    , m_sizeForSave(rhs.m_sizeForSave)
    , m_pUserData(rhs.m_pUserData)
    , m_iMagicNum(rhs.m_iMagicNum)
{
}

ImageLoadElement::ImageLoadElement(const QByteArray & rawdata, void * userdata, QString savedFilePath, QSize sizeForSave,uint iMagicNum/*=0*/)
    : m_strFile(savedFilePath)
    , m_rawdata(rawdata)
    , m_sizeForSave(sizeForSave)
    , m_pUserData(userdata)
    , m_iMagicNum(iMagicNum)
{
}

ImageLoadElement & ImageLoadElement::operator =(const ImageLoadElement & rhs)
{
    if(this != &rhs)
    {
        m_strFile = rhs.m_strFile;
        m_rawdata = rhs.m_rawdata;
        m_sizeForSave = rhs.m_sizeForSave;
        m_pUserData = rhs.m_pUserData;
        m_iMagicNum = rhs.m_iMagicNum;
    }

    return *this;
}

bool ImageLoadElement::operator ==(const ImageLoadElement & rhs)
{
    if(this == &rhs) return true;

    return (m_strFile == rhs.m_strFile
            && m_rawdata == rhs.m_rawdata
            && m_pUserData == rhs.m_pUserData
                             );
}

ImageLoadElement::~ImageLoadElement()
{
}

//
// ObserverEvent
//
QEvent::Type ObserverEvent::s_evType = QEvent::None;
ObserverEvent::ObserverEvent(QPointer<QObject> *pointer)
    : QEvent(evType())
    , m_observer(pointer)
{
}

QEvent::Type ObserverEvent::evType()
{
    if(s_evType == QEvent::None)
    {
        s_evType = (QEvent::Type)registerEventType();
    }

    return s_evType;
}

//
// NewTaskEvent
//

QEvent::Type NewTaskEvent::s_evType = QEvent::None;
NewTaskEvent::NewTaskEvent()
    : QEvent(evType())
{
}

QEvent::Type NewTaskEvent::evType()
{
    if(s_evType == QEvent::None)
    {
        s_evType = (QEvent::Type)registerEventType();
    }

    return s_evType;
}

//
// class ImageLoader
//
ImageLoader::ImageLoader(ImageLoadManager * mgr)
    : m_manager(mgr), m_observer(0)
{
}

bool ImageLoader::event(QEvent *e)
{
    if(e->type() == ObserverEvent::evType())
    {
        m_observer = ((ObserverEvent*)e)->m_observer;
    }
    else if(e->type() == NewTaskEvent::evType())
    {
        if(loadOneImage() > 0)
        {
            QCoreApplication::postEvent(this, new NewTaskEvent());
        }
    }
    else
    {
        return QObject::event(e);
    }
    return true;
}

int ImageLoader::loadOneImage()
{
    if(m_manager.isNull()) return 0;
    m_manager->m_mutexForLoadList.lock();
    if(m_manager->m_loadList.isEmpty()){
        m_manager->m_mutexForLoadList.unlock();
        return 0;
    }
    ImageLoadElement element(m_manager->m_loadList.front());
    m_manager->m_mutexForLoadList.unlock();

    //perform image loading
    QByteArray data(element.rawdata());
    bool loaded = false;
    QString strFile = element.filePath();
    QImage * image = 0;
    if(data.isNull())
    {
        //load from file
        if(QFile::exists(strFile) )
        {
            //qDebug() << "load from file " << strFile;
            image = new QImage;
            loaded = image->load(strFile);
        }
    }
    else
    {
        //load from data
        //qDebug() << "load from raw data";
        image = new QImage;
        loaded = image->loadFromData(data);
    }

    //scaled pixmap to sizeScale
    QSize sizeScale = element.sizeForSave();
    QImage * scaledImage = 0;
    if(loaded && !sizeScale.isEmpty() && sizeScale != image->size())
    {
        scaledImage = new QImage(image->scaled(sizeScale, Qt::KeepAspectRatio));
    }

    //check whether we need
    //remove the loaded element from front of list
    //
    m_manager->m_mutexForLoadList.lock();
    if( m_manager->m_loadList.isEmpty() )
    {
        m_manager->m_mutexForLoadList.unlock();
        //the element we loaded was removed by user.
        SAFE_DELETE(image)
        SAFE_DELETE(scaledImage)
        return 0;
    }
    else
    {
        ImageLoadElement fe(m_manager->m_loadList.front());
        if( fe == element)
        {
            m_manager->m_loadList.pop_front();
        }
        else
        {
            //the loaded element was removed, and other element was added
            m_manager->m_mutexForLoadList.unlock();
            //the element we loaded was removed by user.
            SAFE_DELETE(image)
            SAFE_DELETE(scaledImage)
            return 0;
        }
    }
    m_manager->m_mutexForLoadList.unlock();

    //send ImageLoadedEvent to observer
    if(m_observer != NULL && !(*m_observer).isNull() )
    {
        //qDebug() << "post ImageLoadedEvent to observer";
        QCoreApplication::postEvent((*m_observer).data(),
                                    new ImageLoadedEvent(scaledImage ? *scaledImage : (image ? *image : QImage()),
                                                          element.userdata(), element.filePath(), loaded, element.magicNum()));
    }

    SAFE_DELETE(image)
    SAFE_DELETE(scaledImage)
    return 1;
}

//
// class ImageLoadManager
//
ImageLoadManager::ImageLoadManager(QObject * observer, QObject * parent)
    : QThread(parent)
    , m_bQuit(false)
    , m_observer(new QPointer<QObject>(observer))
{
}

ImageLoadManager::~ImageLoadManager()
{
    //qDebug() << "~ImageLoadManager";
}

void ImageLoadManager::setObserver(QObject * observer)
{
    if(m_loader.isNull())
    {
        if(m_observer != NULL)
        {
            *m_observer = observer;
        }
        else
        {
            m_observer = new QPointer<QObject>(observer);
        }
    }
    else
    {
        QCoreApplication::postEvent(m_loader, new ObserverEvent(new QPointer<QObject>(observer)));
    }
}


void ImageLoadManager::addImageLoadElement(const QString & strFile, void * userdata, QSize sizeForSave,uint imagicNum)
{
    //qDebug() << "addImageLoadElement, file to load " << strFile;
    m_mutexForLoadList.lock();
    m_loadList.append(ImageLoadElement(strFile, userdata, sizeForSave,imagicNum));
    m_mutexForLoadList.unlock();
    if(m_loader)
    {
        QCoreApplication::postEvent(m_loader, new NewTaskEvent);
    }
}


void ImageLoadManager::addImageLoadElement(const ImageLoadElement & element)
{
    m_mutexForLoadList.lock();
    m_loadList.append(element);
    m_mutexForLoadList.unlock();
    if(m_loader)
    {
        QCoreApplication::postEvent(m_loader, new NewTaskEvent);
    }
}

void ImageLoadManager::addImageLoadElement(const QByteArray & rawdata, void * userdata,QString savedFilePath, QSize sizeForSave,uint imagicNum /*= 0*/)
{
    //qDebug() << "addImageLoadElement, raw data";
    m_mutexForLoadList.lock();
    m_loadList.append(ImageLoadElement(rawdata, userdata, savedFilePath, sizeForSave,imagicNum));
    m_mutexForLoadList.unlock();
    if(m_loader)
    {
        QCoreApplication::postEvent(m_loader, new NewTaskEvent);
    }
}


void ImageLoadManager::addImageLoadElement(const QList<ImageLoadElement> & elements)
{
    m_mutexForLoadList.lock();
    m_loadList.append(elements);
    m_mutexForLoadList.unlock();
    if(m_loader)
    {
        QCoreApplication::postEvent(m_loader, new NewTaskEvent);
    }
}

void ImageLoadManager::removeImageLoadElement(const ImageLoadElement & element)
{
    m_mutexForLoadList.lock();
    m_loadList.removeOne(element);
    m_mutexForLoadList.unlock();
}

void ImageLoadManager::removeImageLoadElement(const QList<ImageLoadElement> & elements)
{
    m_mutexForLoadList.lock();
    foreach(ImageLoadElement ple, elements)
    {
        m_loadList.removeOne(ple);
    }
    m_mutexForLoadList.unlock();
}

void ImageLoadManager::removeImageLoadElement(const QString & strFile, void * userdata)
{
    m_mutexForLoadList.lock();
    m_loadList.removeOne(ImageLoadElement(strFile, userdata));
    m_mutexForLoadList.unlock();
}

void ImageLoadManager::removeImageLoadElement(const QByteArray & rawdata, void * userdata, QString savedFilePath)
{
    m_mutexForLoadList.lock();
    m_loadList.removeOne(ImageLoadElement(rawdata, userdata, savedFilePath));
    m_mutexForLoadList.unlock();
}

void ImageLoadManager::abortLoading()
{
    m_mutexForLoadList.lock();
    m_loadList.clear();
    m_mutexForLoadList.unlock();
}

int ImageLoadManager::pendingImageLoadElement()
{
    QMutexLocker locker(&m_mutexForLoadList);
    return m_loadList.size();
}

void ImageLoadManager::run()
{
    qDebug() << "ImageLoadManager running...";
    ImageLoader * loader = new ImageLoader(this);
    m_loader = loader;
    if(m_observer)
    {
        loader->m_observer = m_observer;
        m_observer = 0;
    }
    m_mutexForLoadList.lock();
    if(!m_loadList.isEmpty())
    {
        QCoreApplication::postEvent(m_loader, new NewTaskEvent());
    }
    m_mutexForLoadList.unlock();
    exec();
}
