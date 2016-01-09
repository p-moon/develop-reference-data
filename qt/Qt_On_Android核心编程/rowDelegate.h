#ifndef ROWDELEGATE_H
#define ROWDELEGATE_H
#include <QItemDelegate>

class RowDelegate : public QItemDelegate
{
public:
    RowDelegate(QObject * parent = 0) :QItemDelegate(parent)
    {
    }

    virtual void drawFocus(QPainter *painter, const QStyleOptionViewItem &option,
                           const QRect &rect) const
    {
    }

    virtual void drawCheck(QPainter *painter, const QStyleOptionViewItem &option,
                           const QRect &rect, Qt::CheckState state) const
    {
    }
};
#endif // ROWDELEGATE_H
