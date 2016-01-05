#include "showwidget.h"
#include <QHBoxLayout>
ShowWidget::ShowWidget(QWidget *parent) :
    QWidget(parent)
{
    imageLabel =new QLabel;
    imageLabel->setScaledContents(true);

    text =new QTextEdit;

    QHBoxLayout *mainLayout =new QHBoxLayout(this);
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(text);
}
