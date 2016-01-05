#include "fileview.h"
#include <QStringList>
#include <QIcon>

FileView::FileView(QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    setWindowTitle(tr("File View"));

    fileLineEdit = new QLineEdit(tr("/"));
    fileListWidget = new QListWidget;

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(fileLineEdit);
    mainLayout->addWidget(fileListWidget);

    connect(fileLineEdit,SIGNAL(returnPressed()),this,SLOT(slotShow(QDir)));
    connect(fileListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(slotDirShow(QListWidgetItem*)));

    QString root = "/";
    QDir rootDir(root);
    QStringList string;
    string << "*";
    QFileInfoList list=rootDir.entryInfoList(string);
    showFileInfoList(list);
}

FileView::~FileView()
{
    
}

void FileView::slotShow(QDir dir)
{
    QStringList string;
    string<<"*";
    QFileInfoList list=dir.entryInfoList(string,QDir::AllEntries,QDir::DirsFirst);
    showFileInfoList(list);
}

void FileView::showFileInfoList(QFileInfoList list)
{
    fileListWidget->clear();
    for(unsigned int i=0;i<list.count();i++)
    {
        QFileInfo tmpFileInfo=list.at(i);
        if(tmpFileInfo.isDir())
        {
            QIcon icon("dir.png");
            QString fileName = tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(icon,fileName);
            fileListWidget->addItem(tmp);
        }
        else if(tmpFileInfo.isFile())
        {
            QIcon icon("file.png");
            QString fileName=tmpFileInfo.fileName();
            QListWidgetItem *tmp = new QListWidgetItem(icon,fileName);
            fileListWidget->addItem(tmp);
        }
    }
}

void FileView::slotDirShow(QListWidgetItem * item)
{
    QString str=item->text();
    QDir dir;
    dir.setPath(fileLineEdit->text());
    dir.cd(str);
    fileLineEdit->setText(dir.absolutePath());
    slotShow(dir);
}
