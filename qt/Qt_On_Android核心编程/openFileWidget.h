#ifndef OPENFILEWIDGET_H
#define OPENFILEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include "worker.h"
#include <QFileInfoList>
#include <QIcon>
#include <QFont>

class DirectoryTraverse;
class OpenFileWidget : public QWidget
{
    Q_OBJECT
public:
    OpenFileWidget(const QString &path, QStringList &nameFilters, QWidget *parent = 0);
    ~OpenFileWidget();

    bool event(QEvent *e);

    void getPath(QString & path);
    void getFileList(QStringList & files);

#ifndef ANDROID
    QSize sizeHint() const { return QSize(320,480); }
#endif

    bool onBackKey();

signals:
    void selected(QString strFile);
    void canceled();

protected slots:
    void onItemClicked(QListWidgetItem * item);
    void onItemDoubleClicked(QListWidgetItem * item);
    void onParent();
    void onCancel();
    void onOpen();

protected:
    void setupGUI();
    void updateList(QFileInfoList & files);
    void startTraverse();
    void setupPath(QListWidgetItem *item);
    const QIcon &getFileIcon(const QString &fileName);

private:
    QPushButton *m_parentButton;
    QPushButton *m_openButton;
    QPushButton *m_cancelButton;
    QLabel *m_pathLabel;
    QListWidget *m_fileList;
    QString m_path;
    QStringList m_files;
    WorkerThread *m_worker;
    DirectoryTraverse *m_dirTraverse;
    QIcon m_folderIcon;
    QIcon m_imageFileIcon;
    QIcon m_audioFileIcon;
    QIcon m_videoFileIcon;
    QIcon m_txtFileIcon;
    QIcon m_binFileIcon;
};

#endif // OPENFILEWIDGET_H
