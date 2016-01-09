#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QThread>
#include <QPointer>

class SecondFirer : public QObject
{
    Q_OBJECT
public:
    SecondFirer(int seconds) : m_nSeconds(seconds){}

signals:
    void secondLeft(int sec, unsigned int id);
public slots:
    void onTimeout();
private:
    int m_nSeconds;
};

class CountThread : public QThread
{
public:
    CountThread(QWidget * receiver);
    ~CountThread();

protected:
    void run();

private:
    QPointer<QWidget> m_receiver;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void onSecondLeft(int sec, unsigned int id);

private:
    QLabel * m_label;
};

#endif // WIDGET_H
