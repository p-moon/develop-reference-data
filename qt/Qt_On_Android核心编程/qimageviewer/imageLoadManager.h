#ifndef IMAGELOADMANAGER_H
#define IMAGELOADMANAGER_H
#include <QEvent>
#include <QThread>
#include <QMutex>
#include <QList>
#include <QByteArray>
#include <QSize>
#include <QImage>
#include <QPointer>

#define SAFE_DELETE(p) if(p){ delete p; p = 0;}

class ImageLoadedEvent : public QEvent
{
private:
    ImageLoadedEvent();
    ImageLoadedEvent(const ImageLoadedEvent &);
    ImageLoadedEvent & operator=(const ImageLoadedEvent &);
public:
    ImageLoadedEvent(QImage image, void * userdata, QString filePath, bool bLoaded = false,uint iMagicNum=0);
    ~ImageLoadedEvent();

    bool loaded() const { return m_bLoaded; }
    QString filePath() { return m_strFilePath; }
    QImage image() const { return m_image; }
    void * userdata() const { return m_pUserData; }
    uint magicNum()const{return m_iMagicNum;}

    static QEvent::Type evType();

private:
    void * m_pUserData;
    QImage m_image;
    bool m_bLoaded;
    QString m_strFilePath;
    static QEvent::Type s_evType;
    uint m_iMagicNum;

};

class ImageLoadElement
{
public:
    ImageLoadElement(const QString & filepath,  void * userdata = 0, QSize sizeScale = QSize(), uint iMagicNum=0);
    ImageLoadElement(const QByteArray & rawdata, void * userdata = 0, QString savedFilePath = QString(), QSize sizeScale = QSize(), uint iMagicNum=0);
    ImageLoadElement(const ImageLoadElement & rhs);
    ImageLoadElement & operator=(const ImageLoadElement & rhs);
    bool operator==(const ImageLoadElement & rhs);

    ~ImageLoadElement();

    QString filePath() const { return m_strFile; }
    QByteArray rawdata() const { return m_rawdata; }
    QSize sizeForSave() const { return m_sizeForSave; }
    void * userdata() const { return m_pUserData; }
    uint magicNum()const{return m_iMagicNum;}

protected:
    QString m_strFile;
    QByteArray m_rawdata;
    QSize m_sizeForSave; ///< scaled to this size
    void * m_pUserData;
    uint m_iMagicNum;
};

class ImageLoader;
class ObserverEvent : public QEvent
{
    friend class ImageLoader;
public:
    ObserverEvent(QPointer<QObject> *pointer);

    static QEvent::Type evType();

private:
    static QEvent::Type s_evType;
    QPointer<QObject> *m_observer;
};

class NewTaskEvent : public QEvent
{
public:
    NewTaskEvent();
    static QEvent::Type evType();

private:
    static QEvent::Type s_evType;
};

class ImageLoadManager : public QThread
{    
    friend class ImageLoader;
public:
    ImageLoadManager(QObject * observer, QObject * parent = 0);
    ~ImageLoadManager();

    void addImageLoadElement(const ImageLoadElement & element);
    void addImageLoadElement(const QString & strFile, void * userdata = 0, QSize sizeScale = QSize(),uint imagicNum = 0);
    void addImageLoadElement(const QByteArray & rawdata, void * userdata = 0,QString savedFilePath = QString(), QSize sizeScale = QSize(),uint imagicNum = 0);
    void addImageLoadElement(const QList<ImageLoadElement> & elements);
    void removeImageLoadElement(const QString & strFile, void * userdata);
    void removeImageLoadElement(const QByteArray & rawdata, void * userdata, QString savedFilePath = QString());
    void removeImageLoadElement(const QList<ImageLoadElement> & elements);
    void removeImageLoadElement(const ImageLoadElement & element);
    void abortLoading();
    int pendingImageLoadElement();

    void setObserver(QObject * observer);

protected:
    void run();

private:
    QMutex m_mutexForLoadList;
    QList<ImageLoadElement> m_loadList;
    bool m_bQuit;
    QPointer<QObject> *m_observer;
    QPointer<QObject> m_loader;
};

class ImageLoader : public QObject
{
    friend class ImageLoadManager;
public:
    ImageLoader(ImageLoadManager *mgr);
    bool event(QEvent *e);

private:
    int loadOneImage();

protected:
    QPointer<ImageLoadManager> m_manager;
    QPointer<QObject> *m_observer;
};

#endif // IMAGELOADMANAGER_H
