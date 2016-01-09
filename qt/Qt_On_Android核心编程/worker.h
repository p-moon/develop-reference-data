#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include <QEvent>
#include <QPointer>
#include <QList>

/* NOTES:
 *   1. the caller MUST maintain Runnable's life cycle
 *   2. the derived class MUST offer a QObject instance
 *      to receive RunnableExcutedEvent
 */
class Runnable
{
public:
    Runnable(QObject *observer)
        : m_observer(observer)
    {}
    virtual ~Runnable(){}
    virtual void run() = 0;
    virtual bool notifyAfterRun(){ return true; }

    QPointer<QObject> m_observer;
};

class RunnableExcutedEvent : public QEvent
{
public:
    RunnableExcutedEvent(Runnable *r);
    Runnable *m_runnable;

    static QEvent::Type evType();

private:
    static QEvent::Type s_evType;
};

class WorkerThread : public QThread
{
public:
    WorkerThread(QObject *parent = 0);
    ~WorkerThread();

    void postRunnable(Runnable *r);

protected:
    void run();

private:
    QPointer<QObject> m_worker;
    QList<Runnable*> *m_runnables; //temp queue
};

class Worker : public QObject
{
    friend class WorkerThread;
public:
    Worker() : m_runnables(0)
    {}
    ~Worker();

    bool event(QEvent *e);

private:
    void excuteQueuedRunnables();
    void excuteRunnable(Runnable *runnable);

private:
    QList<Runnable*> *m_runnables;
};

#endif // WORKER_H
