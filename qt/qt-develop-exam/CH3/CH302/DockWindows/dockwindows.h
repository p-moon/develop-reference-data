#ifndef DOCKWINDOWS_H
#define DOCKWINDOWS_H

#include <QMainWindow>

class DockWindows : public QMainWindow
{
    Q_OBJECT
    
public:
    DockWindows(QWidget *parent = 0);
    ~DockWindows();
};

#endif // DOCKWINDOWS_H
