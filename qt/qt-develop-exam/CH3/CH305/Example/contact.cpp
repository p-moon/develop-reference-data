#include "contact.h"

Contact::Contact(QWidget *parent) :
    QWidget(parent)
{
    EmailLabel =new QLabel(tr("电子邮件："));
    EmailLineEdit =new QLineEdit;

    AddrLabel =new QLabel(tr("联系地址："));
    AddrLineEdit =new QLineEdit;

    CodeLabel =new QLabel(tr("邮政编码："));
    CodeLineEdit =new QLineEdit;

    MoviTelLabel =new QLabel(tr("移动电话："));
    MoviTelLineEdit =new QLineEdit;
    MoviTelCheckBook =new QCheckBox(tr("接收留言"));

    ProTelLabel =new QLabel(tr("办公电话："));
    ProTelLineEdit =new QLineEdit;

    mainLayout =new QGridLayout(this);
    mainLayout->setMargin(15);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(EmailLabel,0,0);
    mainLayout->addWidget(EmailLineEdit,0,1);
    mainLayout->addWidget(AddrLabel,1,0);
    mainLayout->addWidget(AddrLineEdit,1,1);
    mainLayout->addWidget(CodeLabel,2,0);
    mainLayout->addWidget(CodeLineEdit,2,1);
    mainLayout->addWidget(MoviTelLabel,3,0);
    mainLayout->addWidget(MoviTelLineEdit,3,1);
    mainLayout->addWidget(MoviTelCheckBook,3,2);
    mainLayout->addWidget(ProTelLabel,4,0);
    mainLayout->addWidget(ProTelLineEdit,4,1);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
}
