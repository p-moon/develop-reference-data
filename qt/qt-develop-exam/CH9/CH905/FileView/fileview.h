#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDir>
#include <QListWidgetItem>
#include <QFileInfoList>

class FileView : public QDialog
{
    Q_OBJECT
    
public:
    FileView(QWidget *parent = 0,Qt::WindowFlags f=0);
    ~FileView();    
    void showFileInfoList(QFileInfoList list);

public slots:
    void slotShow(QDir dir);
    void slotDirShow(QListWidgetItem * item);

private:
    QLineEdit *fileLineEdit;
    QListWidget *fileListWidget;

    QVBoxLayout *mainLayout;
};

#endif // FILEVIEW_H
