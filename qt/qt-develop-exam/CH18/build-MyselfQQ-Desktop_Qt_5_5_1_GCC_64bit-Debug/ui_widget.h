/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QTextBrowser *msgBrowser;
    QHBoxLayout *horizontalLayout;
    QFontComboBox *fontCbx;
    QComboBox *sizeCbx;
    QToolButton *boldTBtn;
    QToolButton *italicTBtn;
    QToolButton *underlineTBtn;
    QToolButton *colorTBtn;
    QToolButton *sendTBtn;
    QToolButton *saveTBtn;
    QToolButton *clearTBtn;
    QTextEdit *msgTxtEdit;
    QTableWidget *usrTblWidget;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *sendBtn;
    QSpacerItem *horizontalSpacer_2;
    QLabel *usrNumLbl;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *exitBtn;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(730, 450);
        verticalLayout_2 = new QVBoxLayout(Widget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(3);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        msgBrowser = new QTextBrowser(Widget);
        msgBrowser->setObjectName(QStringLiteral("msgBrowser"));

        verticalLayout->addWidget(msgBrowser);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        fontCbx = new QFontComboBox(Widget);
        fontCbx->setObjectName(QStringLiteral("fontCbx"));

        horizontalLayout->addWidget(fontCbx);

        sizeCbx = new QComboBox(Widget);
        sizeCbx->setObjectName(QStringLiteral("sizeCbx"));

        horizontalLayout->addWidget(sizeCbx);

        boldTBtn = new QToolButton(Widget);
        boldTBtn->setObjectName(QStringLiteral("boldTBtn"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/bold.png"), QSize(), QIcon::Normal, QIcon::Off);
        boldTBtn->setIcon(icon);
        boldTBtn->setIconSize(QSize(26, 26));
        boldTBtn->setCheckable(true);
        boldTBtn->setAutoRaise(true);

        horizontalLayout->addWidget(boldTBtn);

        italicTBtn = new QToolButton(Widget);
        italicTBtn->setObjectName(QStringLiteral("italicTBtn"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/italic.png"), QSize(), QIcon::Normal, QIcon::Off);
        italicTBtn->setIcon(icon1);
        italicTBtn->setIconSize(QSize(26, 26));
        italicTBtn->setCheckable(true);
        italicTBtn->setAutoRaise(true);

        horizontalLayout->addWidget(italicTBtn);

        underlineTBtn = new QToolButton(Widget);
        underlineTBtn->setObjectName(QStringLiteral("underlineTBtn"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/under.png"), QSize(), QIcon::Normal, QIcon::Off);
        underlineTBtn->setIcon(icon2);
        underlineTBtn->setIconSize(QSize(26, 26));
        underlineTBtn->setCheckable(true);
        underlineTBtn->setAutoRaise(true);

        horizontalLayout->addWidget(underlineTBtn);

        colorTBtn = new QToolButton(Widget);
        colorTBtn->setObjectName(QStringLiteral("colorTBtn"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/color.png"), QSize(), QIcon::Normal, QIcon::Off);
        colorTBtn->setIcon(icon3);
        colorTBtn->setIconSize(QSize(26, 26));
        colorTBtn->setCheckable(false);
        colorTBtn->setAutoRaise(true);

        horizontalLayout->addWidget(colorTBtn);

        sendTBtn = new QToolButton(Widget);
        sendTBtn->setObjectName(QStringLiteral("sendTBtn"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/send.png"), QSize(), QIcon::Normal, QIcon::Off);
        sendTBtn->setIcon(icon4);
        sendTBtn->setIconSize(QSize(26, 26));
        sendTBtn->setCheckable(false);
        sendTBtn->setAutoRaise(true);

        horizontalLayout->addWidget(sendTBtn);

        saveTBtn = new QToolButton(Widget);
        saveTBtn->setObjectName(QStringLiteral("saveTBtn"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveTBtn->setIcon(icon5);
        saveTBtn->setIconSize(QSize(26, 26));
        saveTBtn->setCheckable(false);
        saveTBtn->setAutoRaise(true);

        horizontalLayout->addWidget(saveTBtn);

        clearTBtn = new QToolButton(Widget);
        clearTBtn->setObjectName(QStringLiteral("clearTBtn"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearTBtn->setIcon(icon6);
        clearTBtn->setIconSize(QSize(26, 26));
        clearTBtn->setCheckable(false);
        clearTBtn->setAutoRaise(true);

        horizontalLayout->addWidget(clearTBtn);


        verticalLayout->addLayout(horizontalLayout);

        msgTxtEdit = new QTextEdit(Widget);
        msgTxtEdit->setObjectName(QStringLiteral("msgTxtEdit"));

        verticalLayout->addWidget(msgTxtEdit);

        verticalLayout->setStretch(0, 5);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 2);

        horizontalLayout_2->addLayout(verticalLayout);

        usrTblWidget = new QTableWidget(Widget);
        if (usrTblWidget->columnCount() < 2)
            usrTblWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        usrTblWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        usrTblWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        usrTblWidget->setObjectName(QStringLiteral("usrTblWidget"));
        usrTblWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        usrTblWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        usrTblWidget->setShowGrid(false);

        horizontalLayout_2->addWidget(usrTblWidget);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(78, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        sendBtn = new QPushButton(Widget);
        sendBtn->setObjectName(QStringLiteral("sendBtn"));

        horizontalLayout_3->addWidget(sendBtn);

        horizontalSpacer_2 = new QSpacerItem(188, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        usrNumLbl = new QLabel(Widget);
        usrNumLbl->setObjectName(QStringLiteral("usrNumLbl"));

        horizontalLayout_3->addWidget(usrNumLbl);

        horizontalSpacer_3 = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        exitBtn = new QPushButton(Widget);
        exitBtn->setObjectName(QStringLiteral("exitBtn"));

        horizontalLayout_3->addWidget(exitBtn);

        horizontalLayout_3->setStretch(0, 5);
        horizontalLayout_3->setStretch(1, 2);
        horizontalLayout_3->setStretch(2, 5);
        horizontalLayout_3->setStretch(3, 3);
        horizontalLayout_3->setStretch(4, 2);
        horizontalLayout_3->setStretch(5, 2);

        verticalLayout_2->addLayout(horizontalLayout_3);


        retranslateUi(Widget);

        sizeCbx->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        sizeCbx->clear();
        sizeCbx->insertItems(0, QStringList()
         << QApplication::translate("Widget", "8", 0)
         << QApplication::translate("Widget", "9", 0)
         << QApplication::translate("Widget", "10", 0)
         << QApplication::translate("Widget", "11", 0)
         << QApplication::translate("Widget", "12", 0)
         << QApplication::translate("Widget", "13", 0)
         << QApplication::translate("Widget", "14", 0)
         << QApplication::translate("Widget", "15", 0)
         << QApplication::translate("Widget", "16", 0)
         << QApplication::translate("Widget", "17", 0)
         << QApplication::translate("Widget", "18", 0)
         << QApplication::translate("Widget", "19", 0)
         << QApplication::translate("Widget", "20", 0)
         << QApplication::translate("Widget", "21", 0)
         << QApplication::translate("Widget", "22", 0)
        );
#ifndef QT_NO_TOOLTIP
        boldTBtn->setToolTip(QApplication::translate("Widget", "\345\212\240\347\262\227", 0));
#endif // QT_NO_TOOLTIP
        boldTBtn->setText(QApplication::translate("Widget", "...", 0));
#ifndef QT_NO_TOOLTIP
        italicTBtn->setToolTip(QApplication::translate("Widget", "\345\200\276\346\226\234", 0));
#endif // QT_NO_TOOLTIP
        italicTBtn->setText(QApplication::translate("Widget", "...", 0));
#ifndef QT_NO_TOOLTIP
        underlineTBtn->setToolTip(QApplication::translate("Widget", "\344\270\213\345\210\222\347\272\277", 0));
#endif // QT_NO_TOOLTIP
        underlineTBtn->setText(QApplication::translate("Widget", "...", 0));
#ifndef QT_NO_TOOLTIP
        colorTBtn->setToolTip(QApplication::translate("Widget", "\346\233\264\346\224\271\345\255\227\344\275\223\351\242\234\350\211\262", 0));
#endif // QT_NO_TOOLTIP
        colorTBtn->setText(QApplication::translate("Widget", "...", 0));
#ifndef QT_NO_TOOLTIP
        sendTBtn->setToolTip(QApplication::translate("Widget", "\344\274\240\350\276\223\346\226\207\344\273\266", 0));
#endif // QT_NO_TOOLTIP
        sendTBtn->setText(QApplication::translate("Widget", "...", 0));
#ifndef QT_NO_TOOLTIP
        saveTBtn->setToolTip(QApplication::translate("Widget", "\344\277\235\345\255\230\350\201\212\345\244\251\350\256\260\345\275\225", 0));
#endif // QT_NO_TOOLTIP
        saveTBtn->setText(QApplication::translate("Widget", "...", 0));
#ifndef QT_NO_TOOLTIP
        clearTBtn->setToolTip(QApplication::translate("Widget", "\346\270\205\347\251\272\350\201\212\345\244\251\350\256\260\345\275\225", 0));
#endif // QT_NO_TOOLTIP
        clearTBtn->setText(QApplication::translate("Widget", "...", 0));
        QTableWidgetItem *___qtablewidgetitem = usrTblWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Widget", "\347\224\250\346\210\267\345\220\215", 0));
        QTableWidgetItem *___qtablewidgetitem1 = usrTblWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Widget", "IP\345\234\260\345\235\200", 0));
        sendBtn->setText(QApplication::translate("Widget", "\345\217\221\351\200\201", 0));
        usrNumLbl->setText(QApplication::translate("Widget", "\345\234\250\347\272\277\347\224\250\346\210\267\357\274\2320\344\272\272", 0));
        exitBtn->setText(QApplication::translate("Widget", "\351\200\200\345\207\272", 0));
        Q_UNUSED(Widget);
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
