#include "mainwindow.h"
#include <QItemSelectionModel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createAction();
    createMenu();
    setupModel();
    setupView();

    setWindowTitle(tr("View Example"));
    resize(600,600);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::createAction()
{
    openAct = new QAction(tr("打开"),this);
    connect(openAct,SIGNAL(triggered()),this,SLOT(slotOpen()));
}

void MainWindow::createMenu()
{
    fileMenu = new QMenu(tr("文件"),this);
    fileMenu->addAction(openAct);
    menuBar()->addMenu(fileMenu);
}

void MainWindow::setupModel()
{
    model = new QStandardItemModel(4,4,this);
    model->setHeaderData(0,Qt::Horizontal,tr("部门"));
    model->setHeaderData(1,Qt::Horizontal,tr("男"));
    model->setHeaderData(2,Qt::Horizontal,tr("女"));
    model->setHeaderData(3,Qt::Horizontal,tr("退休"));
}

void MainWindow::setupView()
{
    splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);

    histogram = new HistogramView(splitter);

    histogram->setModel(model);
    table = new QTableView;
    table->setModel(model);

    QItemSelectionModel *selectionModel=new QItemSelectionModel(model);
    table->setSelectionModel(selectionModel);
    histogram->setSelectionModel(selectionModel);

    splitter->addWidget(table);
    splitter->addWidget(histogram);


    setCentralWidget(splitter);

    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),table,SLOT(selectionChanged(QItemSelection,QItemSelection)));
    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),histogram,SLOT(selectionChanged(QItemSelection,QItemSelection)));
}

void MainWindow::slotOpen()
{
    QString name;
    name = QFileDialog::getOpenFileName(this,"打开",".","histogram files (*.txt)");
    if (!name.isEmpty())
        openFile(name);
}

void MainWindow::openFile(QString path)
{
    if (!path.isEmpty())
    {
        QFile file(path);
        if (file.open(QFile::ReadOnly|QFile::Text))
        {
            QTextStream stream(&file);
            QString line;

            model->removeRows(0,model->rowCount(QModelIndex()),QModelIndex());
            int row = 0;
            do
            {
                line = stream.readLine();
                if (!line.isEmpty())
                {
                    model->insertRows(row, 1, QModelIndex());
                    QStringList pieces=line.split(",",QString::SkipEmptyParts);
                    model->setData(model->index(row, 0, QModelIndex()),pieces.value(0));
                    model->setData(model->index(row, 1, QModelIndex()),pieces.value(1));
                    model->setData(model->index(row, 2, QModelIndex()),pieces.value(2));
                    model->setData(model->index(row, 3, QModelIndex()),pieces.value(3));
                    row++;
                }
            }while (!line.isEmpty());
            file.close();
        }
    }
}
