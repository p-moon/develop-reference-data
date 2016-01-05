#include "drawer.h"
#include <QGroupBox>
#include <QVBoxLayout>

Drawer::Drawer(QWidget *parent, Qt::WindowFlags f)
    :QToolBox(parent,f)
{
    setWindowTitle(tr("Myself QQ 2013"));                   //设置主窗体的标题
    setWindowIcon(QPixmap(":/images/qq.png"));              //设置主窗体标题栏图标

    toolBtn1 =new QToolButton;
    toolBtn1->setText(tr("水漂奇鼋"));
    toolBtn1->setIcon(QPixmap(":/images/spqy.png"));
    toolBtn1->setIconSize(QPixmap(":/images/spqy.png").size());
    toolBtn1->setAutoRaise(true);
    toolBtn1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn1,SIGNAL(clicked()),this,SLOT(showChatWidget1()));

    toolBtn2 =new QToolButton;
    toolBtn2->setText(tr("忆梦如澜"));
    toolBtn2->setIcon(QPixmap(":/images/ymrl.png"));
    toolBtn2->setIconSize(QPixmap(":/images/ymrl.png").size());
    toolBtn2->setAutoRaise(true);
    toolBtn2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn2,SIGNAL(clicked()),this,SLOT(showChatWidget2()));

    toolBtn3 =new QToolButton;
    toolBtn3->setText(tr("北京出版人"));
    toolBtn3->setIcon(QPixmap(":/images/qq.png"));
    toolBtn3->setIconSize(QPixmap(":/images/qq.png").size());
    toolBtn3->setAutoRaise(true);
    toolBtn3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn3,SIGNAL(clicked()),this,SLOT(showChatWidget3()));

    toolBtn4 =new QToolButton;
    toolBtn4->setText(tr("Cherry"));
    toolBtn4->setIcon(QPixmap(":/images/Cherry.png"));
    toolBtn4->setIconSize(QPixmap(":/images/Cherry.png").size());
    toolBtn4->setAutoRaise(true);
    toolBtn4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn4,SIGNAL(clicked()),this,SLOT(showChatWidget4()));

    toolBtn5 =new QToolButton;
    toolBtn5->setText(tr("淡然"));
    toolBtn5->setIcon(QPixmap(":/images/dr.png"));
    toolBtn5->setIconSize(QPixmap(":/images/dr.png").size());
    toolBtn5->setAutoRaise(true);
    toolBtn5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn5,SIGNAL(clicked()),this,SLOT(showChatWidget5()));

    toolBtn6 =new QToolButton;
    toolBtn6->setText(tr("娇娇girl"));
    toolBtn6->setIcon(QPixmap(":/images/jj.png"));
    toolBtn6->setIconSize(QPixmap(":/images/jj.png").size());
    toolBtn6->setAutoRaise(true);
    toolBtn6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn6,SIGNAL(clicked()),this,SLOT(showChatWidget6()));

    toolBtn7 =new QToolButton;
    toolBtn7->setText(tr("落水无痕"));
    toolBtn7->setIcon(QPixmap(":/images/lswh.png"));
    toolBtn7->setIconSize(QPixmap(":/images/lswh.png").size());
    toolBtn7->setAutoRaise(true);
    toolBtn7->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn7,SIGNAL(clicked()),this,SLOT(showChatWidget7()));

    toolBtn8 =new QToolButton;
    toolBtn8->setText(tr("青墨暖暖"));
    toolBtn8->setIcon(QPixmap(":/images/qmnn.png"));
    toolBtn8->setIconSize(QPixmap(":/images/qmnn.png").size());
    toolBtn8->setAutoRaise(true);
    toolBtn8->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn8,SIGNAL(clicked()),this,SLOT(showChatWidget8()));

    toolBtn9 =new QToolButton;
    toolBtn9->setText(tr("无语"));
    toolBtn9->setIcon(QPixmap(":/images/wy.png"));
    toolBtn9->setIconSize(QPixmap(":/images/wy.png").size());
    toolBtn9->setAutoRaise(true);
    toolBtn9->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(toolBtn9,SIGNAL(clicked()),this,SLOT(showChatWidget9()));

    QGroupBox *groupBox=new QGroupBox;
    QVBoxLayout *layout=new QVBoxLayout(groupBox);
    layout->setMargin(20);//布局中各窗体的显示间距
    layout->setAlignment(Qt::AlignLeft);//布局中各窗体的显示位置
    layout->addWidget(toolBtn1);
    layout->addWidget(toolBtn2);
    layout->addWidget(toolBtn3);
    layout->addWidget(toolBtn4);
    layout->addWidget(toolBtn5);
    layout->addWidget(toolBtn6);
    layout->addWidget(toolBtn7);
    layout->addWidget(toolBtn8);
    layout->addWidget(toolBtn9);
    layout->addStretch();//插入一个占位符

    this->addItem((QWidget*)groupBox,tr("群成员"));    
}

void Drawer::showChatWidget1()
{
    chatWidget1 = new Widget(0,toolBtn1->text());
    chatWidget1->setWindowTitle(toolBtn1->text());
    chatWidget1->setWindowIcon(toolBtn1->icon());
    chatWidget1->show();
}

void Drawer::showChatWidget2()
{
    chatWidget2 = new Widget(0,toolBtn2->text());
    chatWidget2->setWindowTitle(toolBtn2->text());
    chatWidget2->setWindowIcon(toolBtn2->icon());
    chatWidget2->show();
}

void Drawer::showChatWidget3()
{
    chatWidget3 = new Widget(0,toolBtn3->text());
    chatWidget3->setWindowTitle(toolBtn3->text());
    chatWidget3->setWindowIcon(toolBtn3->icon());
    chatWidget3->show();
}

void Drawer::showChatWidget4()
{
    chatWidget4 = new Widget(0,toolBtn4->text());
    chatWidget4->setWindowTitle(toolBtn4->text());
    chatWidget4->setWindowIcon(toolBtn4->icon());
    chatWidget4->show();
}

void Drawer::showChatWidget5()
{
    chatWidget5 = new Widget(0,toolBtn5->text());
    chatWidget5->setWindowTitle(toolBtn5->text());
    chatWidget5->setWindowIcon(toolBtn5->icon());
    chatWidget5->show();
}

void Drawer::showChatWidget6()
{
    chatWidget6 = new Widget(0,toolBtn6->text());
    chatWidget6->setWindowTitle(toolBtn6->text());
    chatWidget6->setWindowIcon(toolBtn6->icon());
    chatWidget6->show();
}

void Drawer::showChatWidget7()
{
    chatWidget7 = new Widget(0,toolBtn7->text());
    chatWidget7->setWindowTitle(toolBtn7->text());
    chatWidget7->setWindowIcon(toolBtn7->icon());
    chatWidget7->show();
}

void Drawer::showChatWidget8()
{
    chatWidget8 = new Widget(0,toolBtn8->text());
    chatWidget8->setWindowTitle(toolBtn8->text());
    chatWidget8->setWindowIcon(toolBtn8->icon());
    chatWidget8->show();
}

void Drawer::showChatWidget9()
{
    chatWidget9 = new Widget(0,toolBtn9->text());
    chatWidget9->setWindowTitle(toolBtn9->text());
    chatWidget9->setWindowIcon(toolBtn9->icon());
    chatWidget9->show();
}
