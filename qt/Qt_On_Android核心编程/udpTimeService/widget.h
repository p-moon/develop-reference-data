#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onGetTimeButtonClicked();

protected:
    QLineEdit * m_serverEdit;
    QPushButton *m_getTimeButton;
    QLabel *m_timeLabel;
};

#endif // WIDGET_H
