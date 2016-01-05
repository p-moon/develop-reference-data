#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QImage>

class ShowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = 0);
    QImage img;
    QLabel *imageLabel;
    QTextEdit *text;
signals:
    
public slots:
    
};

#endif // SHOWWIDGET_H
