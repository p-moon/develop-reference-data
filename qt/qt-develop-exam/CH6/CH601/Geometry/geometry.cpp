#include "geometry.h"

Geometry::Geometry(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Geometry"));
    xLabel =new QLabel(tr("x():"));
    xValueLabel =new QLabel;
    yLabel =new QLabel(tr("y():"));
    yValueLabel =new QLabel;
    FrmLabel =new QLabel(tr("Frame:"));
    FrmValueLabel =new QLabel;
    posLabel =new QLabel(tr("pos():"));
    posValueLabel =new QLabel;
    geoLabel =new QLabel(tr("geometry():"));
    geoValueLabel =new QLabel;
    widthLabel =new QLabel(tr("width():"));
    widthValueLabel =new QLabel;
    heightLabel =new QLabel(tr("height():"));
    heightValueLabel =new QLabel;
    rectLabel =new QLabel(tr("rect():"));
    rectValueLabel =new QLabel;
    sizeLabel =new QLabel(tr("size():"));
    sizeValueLabel =new QLabel;

    mainLayout =new QGridLayout(this);
    mainLayout->addWidget(xLabel,0,0);
    mainLayout->addWidget(xValueLabel,0,1);
    mainLayout->addWidget(yLabel,1,0);
    mainLayout->addWidget(yValueLabel,1,1);
    mainLayout->addWidget(posLabel,2,0);
    mainLayout->addWidget(posValueLabel,2,1);
    mainLayout->addWidget(FrmLabel,3,0);
    mainLayout->addWidget(FrmValueLabel,3,1);
    mainLayout->addWidget(geoLabel,4,0);
    mainLayout->addWidget(geoValueLabel,4,1);
    mainLayout->addWidget(widthLabel,5,0);
    mainLayout->addWidget(widthValueLabel,5,1);
    mainLayout->addWidget(heightLabel,6,0);
    mainLayout->addWidget(heightValueLabel,6,1);
    mainLayout->addWidget(rectLabel,7,0);
    mainLayout->addWidget(rectValueLabel,7,1);
    mainLayout->addWidget(sizeLabel,8,0);
    mainLayout->addWidget(sizeValueLabel,8,1);

    updateLabel();
}

Geometry::~Geometry()
{
    
}

void Geometry::updateLabel()
{
    QString xStr;                      	//获得x()函数的结果并显示
    xValueLabel->setText(xStr.setNum(x()));

    QString yStr;                      	//获得y()函数的结果并显示
    yValueLabel->setText(yStr.setNum(y()));

    QString frameStr;                 	//获得frameGeometry()函数的结果并显示
    QString tempStr1,tempStr2,tempStr3,tempStr4;
    frameStr = tempStr1.setNum(frameGeometry().x())+","+tempStr2.setNum(frameGeometry().y())+","+tempStr3.setNum(frameGeometry().width())+","+tempStr4.setNum(frameGeometry().height());
    FrmValueLabel->setText(frameStr);

    QString positionStr;            	//获得pos()函数的结果并显示
    QString tempStr11,tempStr12;
    positionStr =tempStr11.setNum(pos().x())+","+tempStr12.setNum(pos().y());
    posValueLabel->setText(positionStr);

    QString geoStr;               		//获得geometry()函数的结果并显示
    QString tempStr21,tempStr22,tempStr23,tempStr24;
    geoStr =tempStr21.setNum(geometry().x())+","+tempStr22.setNum(geometry().y())+","+tempStr23.setNum(geometry().width())+","+tempStr24.setNum(geometry().height());
    geoValueLabel->setText(geoStr);

    QString wStr,hStr;                  //获得width()、height()函数的结果并显示
    widthValueLabel->setText(wStr.setNum(width()));
    heightValueLabel->setText(hStr.setNum(height()));

    QString rectStr;                    //获得rect()函数的结果并显示
    QString tempStr31,tempStr32,tempStr33,tempStr34;
    rectStr =tempStr31.setNum(rect().x())+","+tempStr32.setNum(rect().y())+","+tempStr33.setNum(/*rect().width()*/width())+","+tempStr34.setNum(height()/*rect().height()*/);
    rectValueLabel->setText(rectStr);

    QString sizeStr;                 	//获得size()函数的结果并显示
    QString tempStr41,tempStr42;
    sizeStr =tempStr41.setNum(size().width())+","+tempStr42.setNum(size().height());
    sizeValueLabel->setText(sizeStr);
}

void Geometry:: moveEvent(QMoveEvent *)
{
    updateLabel();
}

void Geometry::resizeEvent(QResizeEvent *)
{
    updateLabel();
}
