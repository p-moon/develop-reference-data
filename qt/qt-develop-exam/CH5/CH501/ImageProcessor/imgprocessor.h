#ifndef IMGPROCESSOR_H
#define IMGPROCESSOR_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QComboBox>
#include <QSpinBox>
#include <QToolBar>
#include <QFontComboBox>
#include <QToolButton>
#include <QTextCharFormat>
#include "showwidget.h"

class ImgProcessor : public QMainWindow
{
    Q_OBJECT
    
public:
    ImgProcessor(QWidget *parent = 0);
    ~ImgProcessor();

    void createActions();                        	//创建动作
    void createMenus();                           	//创建菜单
    void createToolBars();                      	//创建工具栏

    void loadFile(QString filename);
    void mergeFormat(QTextCharFormat);

private:
    QMenu *fileMenu;                           		//各项菜单栏
    QMenu *zoomMenu;
    QMenu *rotateMenu;
    QMenu *mirrorMenu;

    QImage img;
    QString fileName;
    ShowWidget *showWidget;

    QAction *openFileAction;                     	//文件菜单项
    QAction *NewFileAction;
    QAction *PrintTextAction;
    QAction *PrintImageAction;
    QAction *exitAction;

    QAction *copyAction;                          	//编辑菜单项
    QAction *cutAction;
    QAction *pasteAction;
    QAction *aboutAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;

    QAction *rotate90Action;                     	//旋转菜单项
    QAction *rotate180Action;
    QAction *rotate270Action;

    QAction *mirrorVerticalAction;              	//镜像菜单项
    QAction *mirrorHorizontalAction;

    QAction *undoAction;
    QAction *redoAction;

    QToolBar *fileTool;                          	//工具栏
    QToolBar *zoomTool;
    QToolBar *rotateTool;
    QToolBar *mirrorTool;

    QToolBar *doToolBar;

    QLabel *fontLabel1;                             //字体设置项
    QFontComboBox *fontComboBox;
    QLabel *fontLabel2;
    QComboBox *sizeComboBox;
    QToolButton *boldBtn;
    QToolButton *italicBtn;
    QToolButton *underlineBtn;
    QToolButton *colorBtn;

    QToolBar *fontToolBar;                          //字体工具栏

    QLabel *listLabel;                              //排序设置项
    QComboBox *listComboBox;
    QActionGroup *actGrp;
    QAction *leftAction;
    QAction *rightAction;
    QAction *centerAction;
    QAction *justifyAction;

    QToolBar *listToolBar;                          //排序工具栏

protected slots:
    void ShowNewFile();
    void ShowOpenFile();
    void ShowPrintText();
    void ShowPrintImage();
    void ShowZoomIn();
    void ShowZoomOut();
    void ShowRotate90();
    void ShowRotate180();
    void ShowRotate270();
    void ShowMirrorVertical();
    void ShowMirrorHorizontal();

    void ShowFontComboBox(QString comboStr);
    void ShowSizeSpinBox(QString spinValue);
    void ShowBoldBtn();
    void ShowItalicBtn();
    void ShowUnderlineBtn();
    void ShowColorBtn();
    void ShowCurrentFormatChanged(const QTextCharFormat &fmt);

    void ShowList(int);
    void ShowAlignment(QAction *act);
    void ShowCursorPositionChanged();
};

#endif // IMGPROCESSOR_H
