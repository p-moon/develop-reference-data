#include "listwidget.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

FileExplorer::FileExplorer(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *modeLayout = new QHBoxLayout();
    mainLayout->addLayout(modeLayout);

    QLabel *label = new QLabel();
    modeLayout->addWidget(label, 1);
    m_modeCombo = new QComboBox();
    m_modeCombo->addItem("List");
    m_modeCombo->addItem("Icon");
    connect(m_modeCombo, SIGNAL(currentTextChanged(QString))
            ,this, SLOT(onViewModeChanged(QString)));
    modeLayout->addWidget(m_modeCombo);
    m_list = new QListWidget();
    connect(m_list, SIGNAL(currentTextChanged(QString)),
            label, SLOT(setText(QString)));
    QListWidgetItem *item = new QListWidgetItem(
                QIcon(":/folder"), "folder");
    item->setTextColor(Qt::blue);
    m_list->addItem(item);
    item = new QListWidgetItem(
                QIcon(":/images/videoFile.png"), "one.mp4");
    m_list->addItem(item);
    item = new QListWidgetItem(
                QIcon(":/images/audioFile.png"), "love.mp3");
    m_list->addItem(item);
    m_list->addItem(new QListWidgetItem(
                        QIcon(":/images/imageFile.png"), "love.mp3"));
    m_list->addItem("text file.txt");
    mainLayout->addWidget(m_list, 1);
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    m_list->setSpacing(6);
    m_list->setCurrentRow(0);
}

void FileExplorer::onViewModeChanged(const QString &mode)
{
    m_list->setViewMode( mode == "List" ?
                             QListView::ListMode :
                             QListView::IconMode );
}
