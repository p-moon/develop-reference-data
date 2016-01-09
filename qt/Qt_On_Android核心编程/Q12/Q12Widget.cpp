#include "Q12Widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

Widget::~Widget()
{
    delete [] m_scores;
}

void Widget::setupUI()
{
    const char* questions[] = {
        "1. 我知道对我的工作要求吗？",
        "2. 我有做好我的工作所需要的\n材料和设备吗？",
        "3. 在工作中，我每天都有机会\n做我最擅长做的事吗？",
        "4. 在过去的七天里，我因工作\n出色而受到表扬吗？",
        "5. 我觉得我的主管或同事关心\n我的个人情况吗？",
        "6. 工作单位有人鼓励我的发展吗？",
        "7. 在工作中，我觉得我的意见受\n到重视吗？",
        "8. 公司的使命目标使我觉得我的\n工作重要吗？",
        "9. 我的同事们致力于高质量的工作吗？",
        "10. 我在工作单位有一个最要好的\n朋友吗？",
        "11. 在过去的六个月内，工作单位\n有人和我谈及我的进步吗？",
        "12. 过去一年里，我在工作中有机会\n学习和成长吗？"
    };
    m_nQuestionCount = sizeof(questions)/sizeof(questions[0]);
    m_scores = new QComboBox*[m_nQuestionCount];

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *line;
    QLabel * question;
    QComboBox *score;
    QStringList scoreList("0");
    scoreList << "1" << "2" << "3" << "4" << "5";

    for(int i = 0; i < m_nQuestionCount; i++)
    {
        line = new QHBoxLayout();
        question = new QLabel(questions[i], this);
        line->addWidget(question, 4, Qt::AlignLeft | Qt::AlignVCenter);
        score = new QComboBox(this);
        score->addItems(scoreList);
        score->setCurrentIndex(3);
        line->addWidget(score, 1, Qt::AlignRight | Qt::AlignVCenter);
        m_scores[i] = score;

        mainLayout->addLayout(line);
    }

    QPushButton * submit = new QPushButton("提交", this);
    connect(submit, SIGNAL(clicked()), this, SLOT(onSubmit()));
    QPushButton * cancel = new QPushButton("取消", this);
    connect(cancel, SIGNAL(clicked()), qApp, SLOT(quit()));
    line = new QHBoxLayout();
    line->addStretch(1);
    line->addWidget(cancel, 0, Qt::AlignRight | Qt::AlignVCenter);
    line->addWidget(submit, 0, Qt::AlignRight | Qt::AlignVCenter);

    mainLayout->addLayout(line);
}

void Widget::onSubmit(){
    QString content;
    for(int i = 0; i < m_nQuestionCount; i++)
    {
        content += QString("问题%1: %2分\n").arg(i+1).arg(m_scores[i]->currentText());
    }

    QMessageBox * msg = new QMessageBox(QMessageBox::Information, "测评结果", content, QMessageBox::Ok, this);
    msg->exec();
}
