#include "widget.h"
#include <QGridLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_ipQuery(this)
{
    connect(&m_ipQuery, SIGNAL(finished(bool,QString,QString))
            ,this, SLOT(onQueryFinished(bool,QString,QString)));
    QGridLayout *layout = new QGridLayout(this);
    layout->setColumnStretch(1, 1);
    QLabel *label = new QLabel("ip:");
    layout->addWidget(label, 0, 0);
    m_ipEdit = new QLineEdit();
    layout->addWidget(m_ipEdit, 0, 1);
    m_queryButton = new QPushButton("查询");
    connect(m_queryButton, SIGNAL(clicked()),
            this, SLOT(onQueryButton()));
    layout->addWidget(m_queryButton, 1, 1);
    m_areaLabel = new QLabel();
    layout->addWidget(m_areaLabel, 2, 0, 1, 2);
    layout->setRowStretch(3, 1);
}

Widget::~Widget()
{

}

void Widget::onQueryButton()
{
    QString ip = m_ipEdit->text();
    if(!ip.isEmpty())
    {
        m_ipQuery.query(ip);
        m_ipEdit->setDisabled(true);
        m_queryButton->setDisabled(true);
    }

}

void Widget::onQueryFinished(bool bOK, QString ip, QString area)
{
    if(bOK)
    {
        m_areaLabel->setText(area);
    }
    else
    {
        m_areaLabel->setText("喔哟，出错了");
    }
    m_ipEdit->setEnabled(true);
    m_queryButton->setEnabled(true);
}
