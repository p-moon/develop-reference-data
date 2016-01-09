#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "imageView.h"
#include <QStackedLayout>
#include "imageProcessor.h"
#include <QList>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    bool event(QEvent *);
    QSize sizeHint() const { return QSize(540, 960);}

protected slots:
    void onBinarizeButton();
    void onNegativeButton();
    void onSharpenButton();
    void onSoftenButton();
    void onGrayButton();
    void onEmbossButton();
    void onOpenFileButton();
    void onCameraButton();
    void onSaveButton();
    void onFileSelected(QString strFile);
    void onFileSelectCanceled();
    void onImageProcessed(QString newFile);
    void onImageLoaded(bool bSuccess);

private:
    void setActionsEnabled(bool enable);
    void setActionsVisible(bool visible);

protected:
    ImageProcessor m_processor;
    QStackedLayout *m_centralLayout;
    ImageView *m_imageView;
    QList<QWidget*> m_controls;
    QString m_origFilePath;
    QString m_tempFilePath;
    QString m_lastOpenPath;
};

#endif // WIDGET_H
