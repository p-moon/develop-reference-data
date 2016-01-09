#ifndef DOORBELL_H
#define DOORBELL_H
#include <QWidget>
#include <QEvent>
#include <QLabel>
#include <QThread>
#include <QPointer>
#include <QStringList>
#include <QTimer>

class BellEvent : public QEvent
{
public:
    BellEvent(const QString & visitor, const QStringList & gifts);
    QString m_visitor;
    QStringList m_gifts;

    static Type eventType();
protected:
    static Type m_evType;
};

class BellThread;
class QTimer;
class Ringer : public QObject
{
    Q_OBJECT
public:
    Ringer(BellThread * t);
public slots:
    void onTimeout();
private:
    BellThread *m_thread;
};

class BellThread : public QThread
{
    friend class Ringer;
public:
    BellThread(QObject * receiver);

protected:
    void run();

private:
    QPointer<QObject> m_receiver;
};

class Master : public QWidget
{
    Q_OBJECT

public:
    Master(QWidget *parent = 0);
    ~Master();
protected:
    bool event(QEvent * e);

private:
    QLabel * m_visitorLabel;
    QLabel * m_movie;
};

#endif // DOORBELL_H
