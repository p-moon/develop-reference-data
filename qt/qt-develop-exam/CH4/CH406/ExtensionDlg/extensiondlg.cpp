#include "extensiondlg.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QHBoxLayout>

ExtensionDlg::ExtensionDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Extension Dialog"));

    createBaseInfo();
    createDetailInfo();

    QVBoxLayout *layout =new QVBoxLayout(this);                 //布局
    layout->addWidget(baseWidget);
    layout->addWidget(detailWidget);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->setSpacing(10);
}

ExtensionDlg::~ExtensionDlg()
{
    
}

void ExtensionDlg::createBaseInfo()
{
    baseWidget =new QWidget;

    QLabel *nameLabel =new QLabel(tr("姓名："));
    QLineEdit *nameLineEdit =new QLineEdit;

    QLabel *sexLabel =new QLabel(tr("性别："));
    QComboBox *sexComboBox =new  QComboBox;
    sexComboBox->insertItem(0,tr("女"));
    sexComboBox->insertItem(1,tr("男"));

    QGridLayout *LeftLayout =new QGridLayout;
    LeftLayout->addWidget(nameLabel,0,0);
    LeftLayout->addWidget(nameLineEdit,0,1);
    LeftLayout->addWidget(sexLabel);
    LeftLayout->addWidget(sexComboBox);

    QPushButton *OKBtn =new QPushButton(tr("确定"));
    QPushButton *DetailBtn =new QPushButton(tr("详细"));
    QDialogButtonBox *btnBox =new QDialogButtonBox(Qt::Vertical);
    btnBox->addButton(OKBtn,QDialogButtonBox::ActionRole);
    btnBox->addButton(DetailBtn,QDialogButtonBox::ActionRole);

    QHBoxLayout *mainLayout =new QHBoxLayout(baseWidget);
    mainLayout->addLayout(LeftLayout);
    mainLayout->addWidget(btnBox);
    connect(DetailBtn,SIGNAL(clicked()),this,SLOT(showDetailInfo()));
}

void ExtensionDlg::createDetailInfo()
{
    detailWidget =new QWidget;

    QLabel *ageLabel =new QLabel(tr("年龄："));
    QLineEdit *ageLineEdit =new QLineEdit;
    ageLineEdit->setText(tr("30"));

    QLabel *departmentLabel =new QLabel(tr("部门："));
    QComboBox *departmentComBox =new QComboBox;
    departmentComBox->addItem(tr("部门1"));
    departmentComBox->addItem(tr("部门2"));
    departmentComBox->addItem(tr("部门3"));
    departmentComBox->addItem(tr("部门4"));

    QLabel *emailLabel =new QLabel(tr("email："));
    QLineEdit *emailLineEdit =new QLineEdit;

    QGridLayout *mainLayout =new QGridLayout(detailWidget);
    mainLayout->addWidget(ageLabel,0,0);
    mainLayout->addWidget(ageLineEdit,0,1);
    mainLayout->addWidget(departmentLabel,1,0);
    mainLayout->addWidget(departmentComBox,1,1);
    mainLayout->addWidget(emailLabel,2,0);
    mainLayout->addWidget(emailLineEdit,2,1);

    detailWidget->hide();
}

void ExtensionDlg::showDetailInfo()
{
    if(detailWidget->isHidden())
        detailWidget->show();
    else
        detailWidget->hide();
}
