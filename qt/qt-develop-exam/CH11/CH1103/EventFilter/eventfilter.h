#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QDialog>
#include <QLabel>
#include <QImage>
#include <QEvent>

class EventFilter : public QDialog
{
    Q_OBJECT
    
public:
    EventFilter(QWidget *parent = 0,Qt::WindowFlags f=0);
    ~EventFilter();
public slots:
    bool eventFilter(QObject *, QEvent *);
private:
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;

    QLabel *stateLabel;

    QImage Image1;
    QImage Image2;
    QImage Image3;
};

#endif // EVENTFILTER_H
