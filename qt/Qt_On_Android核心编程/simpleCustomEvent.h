#ifndef SIMPLECUSTOMEVENT_H
#define SIMPLECUSTOMEVENT_H
#include <QEvent>
#include <QString>

class SimpleCustomEvent : public QEvent
{
public:
    SimpleCustomEvent(int arg1 = 0, const QString &arg2 = QString());
    ~SimpleCustomEvent();

    static Type eventType();

    int m_arg1;
    QString m_arg2;

private:
    static Type m_evType;
};

#endif // SIMPLECUSTOMEVENT_H
