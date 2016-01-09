#ifndef LISTWIDGET_H
#define LISTWIDGET_H
#include <QWidget>
#include <QComboBox>
#include <QListWidget>

class FileExplorer : public QWidget
{
    Q_OBJECT
public:
    FileExplorer(QWidget *parent = 0);

protected slots:
    void onViewModeChanged(const QString &mode);

private:
    QComboBox *m_modeCombo;
    QListWidget *m_list;
};

#endif // LISTWIDGET_H
