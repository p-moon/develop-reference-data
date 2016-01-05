#ifndef PIEVIEW_H
#define PIEVIEW_H

#include <QAbstractItemView>

class PieView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit PieView(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    //为selections赋初值
    void setSelectionModel(QItemSelectionModel *selectionModel);
    QRegion itemRegion(QModelIndex index);

    QRect visualRect(const QModelIndex &index) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint=EnsureVisible);
    QModelIndex indexAt(const QPoint &point) const;
    QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    int horizontalOffset()const;
    int verticalOffset()const;
    bool isIndexHidden(const QModelIndex &index) const;
    void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
private:
    QItemSelectionModel *selections;
    QList<QRegion> RegionList;
signals:
    
public slots:
    
};

#endif // PIEVIEW_H
