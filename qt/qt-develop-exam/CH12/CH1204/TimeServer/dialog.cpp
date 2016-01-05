#include "dialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "timeserver.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("多线程时间服务器"));

    Label1 =new QLabel(tr("服务器端口："));
    Label2 = new QLabel;

    quitBtn = new QPushButton(tr("退出"));
    QHBoxLayout *BtnLayout = new QHBoxLayout;
    BtnLayout->addStretch(1);
    BtnLayout->addWidget(quitBtn);
    BtnLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(Label1);
    mainLayout->addWidget(Label2);
    mainLayout->addLayout(BtnLayout);

    connect(quitBtn,SIGNAL(clicked()),this,SLOT(close()));

    count=0;
    timeServer = new TimeServer(this);
    if(!timeServer->listen())
    {
        QMessageBox::critical(this,tr("多线程时间服务器"),
        tr("无法启动服务器：%1.").arg(timeServer->errorString()));
        close();
        return;
    }
    Label1->setText(tr("服务器端口：%1.").arg(timeServer->serverPort()));
}

Dialog::~Dialog()
{
    
}

void Dialog::slotShow()
{
    Label2->setText(tr("第%1次请求完毕。").arg(++count));
}
