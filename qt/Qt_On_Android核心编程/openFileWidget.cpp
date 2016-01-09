#include "openFileWidget.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDir>
#include <QResizeEvent>

class DirectoryTraverse : public Runnable
{
public:
    DirectoryTraverse(QObject *observer, const QString &path, QStringList &nameFilters)
        : Runnable(observer)
        , m_path(path)
        , m_nameFilters(nameFilters)
    {

    }

    void run()
    {
        QDir dir(m_path);
        m_files = dir.entryInfoList(m_nameFilters,
                                QDir::AllDirs|QDir::Files|QDir::Readable|QDir::NoDotAndDotDot,
                                QDir::Name|QDir::DirsFirst|QDir::IgnoreCase);
    }

    QString m_path;
    QStringList m_nameFilters;
    QFileInfoList m_files;
};

OpenFileWidget::OpenFileWidget(const QString &path
                               , QStringList &nameFilters
                               , QWidget *parent)
    : QWidget(parent)
    , m_path(path)
    , m_dirTraverse(new DirectoryTraverse(this, path, nameFilters))
    , m_folderIcon(":/images/folder.png")
    , m_imageFileIcon(":/images/imageFile.png")
    , m_audioFileIcon(":/images/audioFile.png")
    , m_videoFileIcon(":/images/videoFile.png")
    , m_txtFileIcon(":/images/txtFile.png")
    , m_binFileIcon(":/images/binFile.png")
{
    setFocusPolicy(Qt::StrongFocus);
    setupGUI();
    setFocus();
    m_worker = new WorkerThread();
    m_worker->start();
    m_worker->postRunnable(m_dirTraverse);
    m_pathLabel->setText(m_path);
}

OpenFileWidget::~OpenFileWidget()
{
    m_worker->quit();
    m_worker->wait(1000);
    delete m_dirTraverse;
}

void OpenFileWidget::setupGUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    m_pathLabel = new QLabel(m_path);
    mainLayout->addWidget(m_pathLabel);
    m_fileList = new QListWidget();
    m_fileList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_fileList->setSortingEnabled(false);
    m_fileList->setTextElideMode(Qt::ElideRight);
    m_fileList->setSpacing(4);
    connect(m_fileList, SIGNAL(itemClicked(QListWidgetItem*))
            , this, SLOT(onItemClicked(QListWidgetItem*)));
    connect(m_fileList, SIGNAL(itemDoubleClicked(QListWidgetItem*))
            , this, SLOT(onItemDoubleClicked(QListWidgetItem*)));
    mainLayout->addWidget(m_fileList, 1);
    QHBoxLayout *operateLayout = new QHBoxLayout();
    m_parentButton = new QPushButton("上级目录");
    connect(m_parentButton, SIGNAL(clicked()), this, SLOT(onParent()));
    m_openButton = new QPushButton("打开");
    connect(m_openButton, SIGNAL(clicked()), this, SLOT(onOpen()));
    m_cancelButton = new QPushButton("取消");
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancel()));
    operateLayout->addStretch(1);
    operateLayout->addWidget(m_parentButton);
    operateLayout->addWidget(m_openButton);
    operateLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(operateLayout);
}

bool OpenFileWidget::event(QEvent *e)
{
    if(e->type() == RunnableExcutedEvent::evType())
    {
        updateList(m_dirTraverse->m_files);
        return true;
    }
    return QWidget::event(e);
}

void OpenFileWidget::getPath(QString &path)
{
    path = m_path;
}

void OpenFileWidget::getFileList(QStringList &files)
{
    files = m_files;
}

void OpenFileWidget::onItemClicked(QListWidgetItem *item)
{
    if(item->type() == QListWidgetItem::UserType)
    {
        setupPath(item);
        startTraverse();
    }
    else
    {
        m_fileList->setCurrentItem(item);
    }
}

void OpenFileWidget::onItemDoubleClicked(QListWidgetItem *item)
{
    if(item->type() == QListWidgetItem::UserType)
    {
        setupPath(item);
        startTraverse();
    }
    else
    {
        m_fileList->setCurrentItem(item);
        onOpen();
    }
}

void OpenFileWidget::onParent()
{
    int sep = m_path.lastIndexOf('/');
    if(sep > 0)
    {
#ifdef WIN32
        if(m_path.at(sep - 1) == ':')
        {
            if(m_path.length() == sep + 1) return;
            m_path = m_path.left(sep + 1);
        }
        else m_path = m_path.left(sep);
#elif ANDROID
        m_path = m_path.left(sep);
        if(m_path == "/data/data") m_path = "/";
#endif


        startTraverse();
    }
    else if(sep == 0)
    {
        if(m_path.length() > 1)
        {
            m_path = "/";
            startTraverse();
        }
    }
}

void OpenFileWidget::onOpen()
{
    QListWidgetItem * item = m_fileList->currentItem();
    if(item && item->type() == 0)
    {
        QString fileName = item->text();
        emit selected(fileName);
        close();
    }
}

bool OpenFileWidget::onBackKey()
{
    onCancel();
    return true;
}

void OpenFileWidget::onCancel()
{
    emit canceled();
    close();
}

void OpenFileWidget::updateList(QFileInfoList & files)
{
    int count = files.size();
    const QFileInfo *fi;
    QListWidgetItem *item;
    for(int i = 0; i < count; i++)
    {
        fi = &files.at(i);
        if(fi->isDir())
        {
            item = new QListWidgetItem(m_folderIcon
                                       , fi->fileName()
                                       , 0, QListWidgetItem::UserType);
        }
        else
        {
            QString fileName = fi->fileName();
            item = new QListWidgetItem(getFileIcon(fileName)
                                       , fileName
                                       );
            m_files.append(fileName);
        }
        //item->setFont(m_listItemFont);
        m_fileList->addItem(item);
    }
}

void OpenFileWidget::startTraverse()
{
    m_dirTraverse->m_path = m_path;
    m_dirTraverse->m_files.clear();
    m_worker->postRunnable(m_dirTraverse);
    m_pathLabel->setText(m_path);
    m_fileList->clear();
    m_files.clear();
}


void OpenFileWidget::setupPath(QListWidgetItem *item)
{
    QString dirName = item->text();
#ifndef WIN32
    if(m_path == "/")
    {
        if(dirName == "data")
        {
            m_path = QDir::currentPath();
        }
        else
        {
            m_path += dirName;
        }
    }
    else
#endif
    {
        if(m_path.endsWith('/'))
        {
            m_path = QString("%1%2").arg(m_path)
                                     .arg(dirName);
        }
        else
        {
            m_path = QString("%1/%2").arg(m_path)
                                     .arg(dirName);
        }
    }
}

const QIcon & OpenFileWidget::getFileIcon(const QString &fileName)
{
    if(fileName.endsWith(".jpg", Qt::CaseInsensitive) ||
            fileName.endsWith(".png", Qt::CaseInsensitive) ||
            fileName.endsWith(".gif", Qt::CaseInsensitive) ||
            fileName.endsWith(".bmp", Qt::CaseInsensitive))
    {
        return m_imageFileIcon;
    }
    else if(fileName.endsWith(".mp3", Qt::CaseInsensitive) ||
            fileName.endsWith(".ogg", Qt::CaseInsensitive) ||
            fileName.endsWith(".wma", Qt::CaseInsensitive) ||
            fileName.endsWith(".wav", Qt::CaseInsensitive) ||
            fileName.endsWith(".mid", Qt::CaseInsensitive) ||
            fileName.endsWith(".ra", Qt::CaseInsensitive) ||
            fileName.endsWith(".aac", Qt::CaseInsensitive))
    {
        return m_audioFileIcon;
    }
    else if(fileName.endsWith(".mp4", Qt::CaseInsensitive) ||
            fileName.endsWith(".mov", Qt::CaseInsensitive) ||
            fileName.endsWith(".m4v", Qt::CaseInsensitive) ||
            fileName.endsWith(".ts", Qt::CaseInsensitive) ||
            fileName.endsWith(".flv", Qt::CaseInsensitive) ||
            fileName.endsWith(".f4v", Qt::CaseInsensitive) ||
            fileName.endsWith(".wmv", Qt::CaseInsensitive) ||
            fileName.endsWith(".asf", Qt::CaseInsensitive) ||
            fileName.endsWith(".avi", Qt::CaseInsensitive) ||
            fileName.endsWith(".mkv", Qt::CaseInsensitive) ||
            fileName.endsWith(".rmvb", Qt::CaseInsensitive) ||
            fileName.endsWith(".rm", Qt::CaseInsensitive) ||
            fileName.endsWith(".3gp", Qt::CaseInsensitive) ||
            fileName.endsWith(".3g2", Qt::CaseInsensitive) ||
            fileName.endsWith(".mpg", Qt::CaseInsensitive) ||
            fileName.endsWith(".mpeg", Qt::CaseInsensitive) ||
            fileName.endsWith(".qt", Qt::CaseInsensitive)
            )
    {
        return m_videoFileIcon;
    }
    else if(fileName.endsWith(".txt", Qt::CaseInsensitive) ||
            fileName.endsWith(".log", Qt::CaseInsensitive) ||
            fileName.endsWith(".xml", Qt::CaseInsensitive) ||
            fileName.endsWith(".html", Qt::CaseInsensitive) ||
            fileName.endsWith(".htm", Qt::CaseInsensitive) ||
            fileName.endsWith(".ini", Qt::CaseInsensitive) ||
            fileName.endsWith(".conf", Qt::CaseInsensitive) ||
            fileName.endsWith(".bat", Qt::CaseInsensitive) ||
            fileName.endsWith(".sh", Qt::CaseInsensitive) ||
            fileName.endsWith(".qml", Qt::CaseInsensitive) ||
            fileName.endsWith(".pro", Qt::CaseInsensitive) ||
            fileName.endsWith(".pri", Qt::CaseInsensitive) ||
            fileName.endsWith(".qrc", Qt::CaseInsensitive) ||
            fileName.endsWith(".c", Qt::CaseInsensitive) ||
            fileName.endsWith(".cpp", Qt::CaseInsensitive) ||
            fileName.endsWith(".h", Qt::CaseInsensitive) ||
            fileName.endsWith(".java", Qt::CaseInsensitive)
            )
    {
        return m_txtFileIcon;
    }
    else
    {
        return m_binFileIcon;
    }
}
