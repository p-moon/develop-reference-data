#include "datedelegate.h"
#include <QDateTimeEdit>

DateDelegate::DateDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *DateDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,const QModelIndex &/*index*/) const
{
    QDateTimeEdit *editor = new QDateTimeEdit(parent);
    editor->setDisplayFormat("yyyy-MM-dd");
    editor->setCalendarPopup(true);
    editor->installEventFilter(const_cast<DateDelegate*>(this));
    return editor;
}

void DateDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    QString dateStr= index.model()->data(index).toString();
    QDate date = QDate::fromString(dateStr,Qt::ISODate);

    QDateTimeEdit *edit=static_cast<QDateTimeEdit*>(editor);
    edit->setDate(date);
}

void DateDelegate::setModelData(QWidget *editor,QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateTimeEdit *edit=static_cast<QDateTimeEdit*>(editor);
    QDate date = edit->date();
    model->setData(index,QVariant(date.toString(Qt::ISODate)));
}

void DateDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
