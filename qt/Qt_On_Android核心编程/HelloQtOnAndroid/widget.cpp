#include "widget.h"
#include <QLabel>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    new QLabel("Hello Qt on Android !", this);
}

Widget::~Widget()
{

}
