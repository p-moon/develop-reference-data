#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();
    void updateParentItem(QTreeWidgetItem* item);

private:
    Ui::Widget *ui;
public slots:
    void treeItemChanged(QTreeWidgetItem* item, int column);
};

#endif // WIDGET_H
