#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qdom.h"
#include <QMainWindow>
#include <QMenuBar>

class QStandardItemModel;
namespace Ui {
class Widget;
}

class Widget : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    enum DateTimeType{Time,Date,DateTime};
    QString getDateTime(DateTimeType type);
    QMenu *manageMenu;
    QMenu *passwordMenu;
    QAction *manageAction;
    QAction *chartAction;
    QAction *quitAction;
    
private slots:
    void on_sellBrandComboBox_currentIndexChanged(const QString &arg1);

    void on_sellNumSpinBox_valueChanged(int arg1);

    void on_sellCancelBtn_clicked();

    void on_sellOkBtn_clicked();

    void on_updateBtn_clicked();

    //void on_manageBtn_clicked();

    //void on_chartBtn_clicked();

    void on_manageMenu_clicked();

    void on_chartMenu_clicked();

    void on_quitMenu_clicked();

    void on_sellFactoryComboBox_currentIndexChanged(const QString &arg1);

    void on_factoryComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::Widget *ui;
    QDomDocument doc;
    bool docRead();
    bool docWrite();
    void writeXml();
    void createNodes(QDomElement &date);
    void showDailyList();
    QStandardItemModel *chartModel;
    void createChartModelView();
    void showChart();
    void createMenuBar();
};

#endif // WIDGET_H
