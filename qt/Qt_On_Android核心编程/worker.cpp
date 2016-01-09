#include "worker.h"
#include <QCoreApplication>

QEvent::Type RunnableExcutedEvent::s_evType = QEvent::None;
RunnableExcutedEvent::RunnableExcutedEvent(Runnable *r)
    : QEvent(evType())
     ,m_runnable(r)
{
}

QEvent::Type RunnableExcutedEvent::evType()
{
    if(s_evType == QEvent::None)
    {
        s_evType = (QEvent::Type)registerEventType();
    }
    return s_evType;
}

class NewRunnableEvent : public QEvent
{
public:
    NewRunnableEvent(Runnable* runnable)
        : QEvent(evType())
        , m_runnable(runnable)
    {}
    static QEvent::Type evType()
    {
        if(s_evType == QEvent::None)
        {
            s_evType = (QEvent::Type)registerEventType();
        }
        return s_evType;
    }

    Runnable * m_runnable;

private:
    static QEvent::Type s_evType;
};
QEvent::Type NewRunnableEvent::s_evType = QEvent::None;

Worker::~Worker()
{
    if(m_runnables)
    {
        delete m_runnables;
    }
}

bool Worker::event(QEvent *e)
{
    QEvent::Type t = e->type();
    if(t == NewRunnableEvent::evType())
    {
        NewRunnableEvent * re = (NewRunnableEvent*)e;
        if(!re->m_runnable)
        {
            excuteQueuedRunnables();
        }
        else
        {
            excuteRunnable(re->m_runnable);
        }
        return true;
    }
    return QObject::event(e);
}

void Worker::excuteQueuedRunnables()
{
    int count = m_runnables ? m_runnables->size() : 0;
    if(count > 0)
    {
        excuteRunnable(m_runnables->takeFirst());
        QCoreApplication::postEvent(this, new NewRunnableEvent(0));
    }
}

void Worker::excuteRunnable(Runnable *runnable)
{
    runnable->run();
    if(runnable->notifyAfterRun() && runnable->m_observer)
    {
        QCoreApplication::postEvent(runnable->m_observer
                                    , new RunnableExcutedEvent(runnable));
    }
}

WorkerThread::WorkerThread(QObject *parent)
    : QThread(parent)
    , m_runnables(0)
{

}

WorkerThread::~WorkerThread()
{
    if(m_runnables)
    {
        delete m_runnables;
    }
}

void WorkerThread::postRunnable(Runnable *r)
{
    if(m_worker)
    {
        QCoreApplication::postEvent(m_worker, new NewRunnableEvent(r));
    }
    else
    {
        m_runnables = new QList<Runnable*>();
        m_runnables->append(r);
    }
}

void WorkerThread::run()
{
    Worker *worker = new Worker();
    m_worker = worker;
    if(m_runnables && m_runnables->size() > 0)
    {
        worker->m_runnables = m_runnables;
        m_runnables = 0;
        QCoreApplication::postEvent(worker, new NewRunnableEvent(0));
    }
    exec();
}
