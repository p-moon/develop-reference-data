#include "fileinfo.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDateTime>

FileInfo::FileInfo(QWidget *parent,Qt::WindowFlags f)
    : QDialog(parent,f)
{
    fileNameLabel = new QLabel(tr("文件名："));
    fileNameLineEdit = new QLineEdit;
    fileBtn = new QPushButton(tr("文件"));

    sizeLabel = new QLabel(tr("大小："));
    sizeLineEdit = new QLineEdit;

    createTimeLabel = new QLabel(tr("创建时间："));
    createTimeLineEdit = new QLineEdit;

    lastModifiedLabel = new QLabel(tr("最后修改时间："));
    lastModifiedLineEdit = new QLineEdit;

    lastReadLabel = new QLabel(tr("最后访问时间："));
    lastReadLineEdit = new QLineEdit;

    propertyLabel = new QLabel(tr("属性："));

    isDirCheckBox = new QCheckBox(tr("目录"));
    isFileCheckBox = new QCheckBox(tr("文件"));
    isSymLinkCheckBox = new QCheckBox(tr("符号连接"));
    isHiddenCheckBox = new QCheckBox(tr("隐藏"));
    isReadableCheckBox = new QCheckBox(tr("读"));
    isWritableCheckBox = new QCheckBox(tr("写"));
    isExecutableCheckBox = new QCheckBox(tr("执行"));

    getBtn = new QPushButton(tr("获得文件信息"));

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(fileNameLabel,0,0);
    gridLayout->addWidget(fileNameLineEdit,0,1);
    gridLayout->addWidget(fileBtn,0,2);

    gridLayout->addWidget(sizeLabel,1,0);
    gridLayout->addWidget(sizeLineEdit,1,1,1,2);

    gridLayout->addWidget(createTimeLabel,2,0);
    gridLayout->addWidget(createTimeLineEdit,2,1,1,2);

    gridLayout->addWidget(lastModifiedLabel,3,0);
    gridLayout->addWidget(lastModifiedLineEdit,3,1,1,2);

    gridLayout->addWidget(lastReadLabel,4,0);
    gridLayout->addWidget(lastReadLineEdit,4,1,1,2);

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(propertyLabel);
    layout2->addStretch();

    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addWidget(isDirCheckBox);
    layout3->addWidget(isFileCheckBox);
    layout3->addWidget(isSymLinkCheckBox);
    layout3->addWidget(isHiddenCheckBox);
    layout3->addWidget(isReadableCheckBox);
    layout3->addWidget(isWritableCheckBox);
    layout3->addWidget(isExecutableCheckBox);

    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addWidget(getBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(layout2);
    mainLayout->addLayout(layout3);
    mainLayout->addLayout(layout4);

    connect(fileBtn,SIGNAL(clicked()),this,SLOT(slotFile()));
    connect(getBtn,SIGNAL(clicked()),this,SLOT(slotGet()));
}

FileInfo::~FileInfo()
{
    
}

void FileInfo::slotFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,"打开","/","files (*)");
    fileNameLineEdit->setText(fileName);
}

void FileInfo::slotGet()
{
    QString file = fileNameLineEdit->text();
    QFileInfo info(file);

    qint64 size = info.size();
    QDateTime created = info.created();
    QDateTime lastModified = info.lastModified();
    QDateTime lastRead = info.lastRead();

    bool isDir = info.isDir();
    bool isFile = info.isFile();
    bool isSymLink = info.isSymLink();
    bool isHidden = info.isHidden();
    bool isReadable = info.isReadable();
    bool isWritable = info.isWritable();
    bool isExecutable = info.isExecutable();

    sizeLineEdit->setText(QString::number(size));
    createTimeLineEdit->setText(created.toString());
    lastModifiedLineEdit->setText(lastModified.toString());
    lastReadLineEdit->setText(lastRead.toString());

    isDirCheckBox->setCheckState(isDir?Qt::Checked:Qt::Unchecked);
    isFileCheckBox->setCheckState(isFile?Qt::Checked:Qt::Unchecked);
    isSymLinkCheckBox->setCheckState(isSymLink?Qt::Checked:Qt::Unchecked);
    isHiddenCheckBox->setCheckState(isHidden?Qt::Checked:Qt::Unchecked);
    isReadableCheckBox->setCheckState(isReadable?Qt::Checked:Qt::Unchecked);
    isWritableCheckBox->setCheckState(isWritable?Qt::Checked:Qt::Unchecked);
    isExecutableCheckBox->setCheckState(isExecutable?Qt::Checked:Qt:: Unchecked);
}
