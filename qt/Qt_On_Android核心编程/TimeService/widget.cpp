#include "widget.h"
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include "timeClient.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(new QLabel("server:"), 0, 0
                      , Qt::AlignVCenter | Qt::AlignRight);
    m_serverEdit = new QLineEdit("192.168.10.102");
    layout->addWidget(m_serverEdit, 0, 1);
    m_getTimeButton = new QPushButton("Get Time");
    connect(m_getTimeButton, SIGNAL(clicked()), this
            , SLOT(onGetTimeButtonClicked()));
    layout->addWidget(m_getTimeButton, 1, 1,
                      Qt::AlignVCenter | Qt::AlignRight);
    m_timeLabel = new QLabel();
    layout->addWidget(m_timeLabel, 2, 0, 1, 2);
    layout->setColumnStretch(1, 1);
    layout->setRowStretch(3, 1);
}

Widget::~Widget()
{

}

void Widget::onGetTimeButtonClicked()
{
    QString server = m_serverEdit->text();
    TimeClient *client = new TimeClient();
    client->connectToServer(server);
    connect(client, SIGNAL(time(QString)),
            m_timeLabel, SLOT(setText(QString)));
    connect(client, SIGNAL(time(QString)),
            client, SLOT(deleteLater()));
}
