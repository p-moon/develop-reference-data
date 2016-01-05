#include "dialog.h"
#include<QGridLayout>
#include<QFileDialog>
#include<QPushButton>
#include <QColorDialog>
#include <QFontDialog>
#include <QMessageBox>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("各种标准对话框的实例"));
    fileBtn=new QPushButton;                                    //各个控件对象的初始化
    fileBtn->setText(tr("文件标准对话框实例"));
    fileLineEdit=new QLineEdit;                                 //用来显示选择的文件名
    mainLayout=new QGridLayout(this);                           //布局设计
    mainLayout->addWidget(fileBtn,0,0);
    mainLayout->addWidget(fileLineEdit,0,1);
    connect(fileBtn,SIGNAL(clicked()),this,SLOT(showFile()));   //事件关联

    colorBtn=new QPushButton;                                   //创建各个控件的对象
    colorBtn->setText(tr("颜色标准对话框实例"));
    colorFrame=new QFrame;
    colorFrame->setFrameShape(QFrame::Box);
    colorFrame->setAutoFillBackground(true);
    mainLayout->addWidget(colorBtn,1,0);                        //布局设计
    mainLayout->addWidget(colorFrame,1,1);
    connect(colorBtn,SIGNAL(clicked()),this,SLOT(showColor())); //事件关联

    fontBtn=new QPushButton;                                	//创建控件的对象
    fontBtn->setText(tr("字体标准对话框实例"));
    fontLineEdit=new QLineEdit;                              	//显示更改的字符串
    fontLineEdit->setText(tr("Welcome!"));
    mainLayout->addWidget(fontBtn,2,0);                         //布局设计
    mainLayout->addWidget(fontLineEdit,2,1);
    connect(fontBtn,SIGNAL(clicked()),this,SLOT(showFont()));   //事件关联

    inputBtn =new QPushButton;                                  //创建控件的对象
    inputBtn->setText(tr("标准输入对话框实例"));
    mainLayout->addWidget(inputBtn,3,0);                        //布局设计
    connect(inputBtn,SIGNAL(clicked()),this,SLOT(showInputDlg()));
                                                                //事件关联

    MsgBtn =new QPushButton;                                    //创建控件对象
    MsgBtn->setText(tr("标准消息对话框实例"));
    mainLayout->addWidget(MsgBtn,3,1);
    connect(MsgBtn,SIGNAL(clicked()),this,SLOT(showMsgDlg()));

    CustomBtn =new QPushButton;
    CustomBtn->setText(tr("用户自定义消息对话框实例"));
    label =new QLabel;
    label->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    mainLayout->addWidget(CustomBtn,4,0);
    mainLayout->addWidget(label,4,1);
    connect(CustomBtn,SIGNAL(clicked()),this,SLOT(showCustomDlg()));
}

Dialog::~Dialog()
{
    
}

void Dialog::showFile()
{
    QString s = QFileDialog::getOpenFileName(this,"open file dialog","/","C++ files(*.cpp)::C files(*.c)::Head files(*.h)");
    fileLineEdit->setText(s);
}

void Dialog::showColor()
{
    QColor c = QColorDialog::getColor(Qt::blue);
    if(c.isValid())
    {
        colorFrame->setPalette(QPalette(c));
    }
}

void Dialog::showFont()
{
    bool ok;
    QFont f = QFontDialog::getFont(&ok);
    if (ok)
    {
        fontLineEdit->setFont(f);
    }
}

void Dialog::showInputDlg()
{
    inputDlg =new InputDlg(this);
    inputDlg->show();
}

void Dialog::showMsgDlg()
{
    msgDlg =new MsgBoxDlg();
    msgDlg->show();
}

void Dialog::showCustomDlg()
{
    label->setText(tr("Custom Message Box"));

    QMessageBox customMsgBox;
    customMsgBox.setWindowTitle(tr("用户自定义消息框"));	//设置消息框的标题

    QPushButton *yesBtn=customMsgBox.addButton(tr("Yes"),QMessageBox::ActionRole);
    QPushButton *noBtn=customMsgBox.addButton(tr("No"),QMessageBox::ActionRole);
    QPushButton *cancelBtn=customMsgBox.addButton(QMessageBox::Cancel);

    customMsgBox.setText(tr("这是一个用户自定义消息框!"));
    customMsgBox.setIconPixmap(QPixmap("Qt.png"));
    customMsgBox.exec();

    if(customMsgBox.clickedButton()==yesBtn)
        label->setText("Custom Message Box/Yes");
    if(customMsgBox.clickedButton()==noBtn)
        label->setText("Custom Message Box/No");
    if(customMsgBox.clickedButton()==cancelBtn)
        label->setText("Custom Message Box/Cancel");
    return;
}
