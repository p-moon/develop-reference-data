#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "svgwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createMenu();
public slots:
    void slotOpenFile();
private:
    SvgWindow *svgWindow; 		//以便调用相关函数传递选择的文件名
};

#endif // MAINWINDOW_H
