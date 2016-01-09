#ifndef WORKERTHREADTEMPLATE_H
#define WORKERTHREADTEMPLATE_H
#include <QThread>

//
// WorkerHostThread
//
template <class T>
class WorkerHostThread : public QThread
{
public:
    WorkerHostThread(void * data = 0)
        : workerdata(data)
    {}
    ~WorkerHostThread()
    {}

protected:
    void run()
    {
        T t;
        t.work(workerdata);
        if(t.requireEventLoop())
        {
            exec();
        }
    }

    void * workerdata;
};

#endif // WORKERTEMPLATETHREAD_H
