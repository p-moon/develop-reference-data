#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QVector>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>

class ProgramSkill
{
public:
    ProgramSkill(const QString &name,
                 const QString &description)
        : m_name(name)
        , m_description(description)
        , m_nUseYear(0)
    {}
    QString m_name;
    QString m_description;
    QString m_level;
    int m_nUseYear;
};

class ProgramSkillWidget : public QWidget
{
    Q_OBJECT

public:
    ProgramSkillWidget(QWidget *parent = 0);
    ~ProgramSkillWidget();

protected slots:
    void onCellClicked(int row, int column);
    void onLevelChanged(const QString &text);
    void onYearsChanged(const QString &text);
    void onSubmit();

protected:
    QString evaluate();

private:
    void setupProgramSkillTable();

protected:
    QVector<ProgramSkill*> m_skills;
    QVector<QComboBox*> m_levels;
    QVector<QLineEdit*> m_years;
    QTableWidget *m_skillTable;
    QPushButton *m_submitButton;
};

#endif // TABLEWIDGET_H
