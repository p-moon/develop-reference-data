#ifndef Q12WIDGET_H
#define Q12WIDGET_H

#include <QWidget>
#include <QComboBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void setupUI();

protected slots:
    void onSubmit();

private:
    QComboBox ** m_scores;
    int m_nQuestionCount;
};

#endif // Q12WIDGET_H
