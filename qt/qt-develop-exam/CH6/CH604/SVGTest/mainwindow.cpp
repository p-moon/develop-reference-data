#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("SVG Viewer"));
    createMenu();

    svgWindow =new SvgWindow;
    setCentralWidget(svgWindow);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createMenu()
{
    QMenu *fileMenu =menuBar()->addMenu(tr("文件"));

    QAction *openAct =new QAction(tr("打开"),this);
    connect(openAct,SIGNAL(triggered()),this,SLOT(slotOpenFile()));
    fileMenu->addAction(openAct);
}

void MainWindow::slotOpenFile()
{
    QString name =QFileDialog::getOpenFileName(this,"打开","/","svg files(*.svg)");
    svgWindow->setFile(name);
}
