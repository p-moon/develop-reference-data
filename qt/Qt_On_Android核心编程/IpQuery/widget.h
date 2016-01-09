#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "ipQuery.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected slots:
    void onQueryButton();
    void onQueryFinished(bool bOK, QString ip, QString area);

protected:
    QLineEdit *m_ipEdit;
    QPushButton *m_queryButton;
    QLabel *m_areaLabel;
    IpQuery m_ipQuery;
};

#endif // WIDGET_H
