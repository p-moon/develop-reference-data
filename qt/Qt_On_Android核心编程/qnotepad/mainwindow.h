#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QString>
#include <QList>
#include "note.h"
#include "viewWidget.h"
#include <QPoint>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onNotesUpdate();

protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    void setupGeometry(const QSize & size);
    void releaseGeometryRects();
    void drawBackground(QPainter &painter);
    void drawAddButton(QPainter &painter);
    void drawThumbnails(QPainter &painter);
    void initializeNoteView();
    /* return rect index */
    int rectHitTest(QPoint & pos);
    void newNote();
    void openNote(QNote *note);
    /* return true on newNote() or openNote() */
    bool checkNoteAction(QPoint & pos);
    bool goPrevPage();
    bool goNextPage();
    void releaseNotes();

private:
    QList<QNote*> m_notes;
    QPixmap *m_background;
    QPixmap *m_thumbBackground;
    QPixmap *m_thumbAdd;
    QList<QRect*> m_thumbRects;
    int m_nStartNote;
    int m_nEndNote;

    NoteViewWidget *m_noteView;
    QPen m_framePen;
    QPen m_focusFramePen;
    int m_nPressRect;
    QPoint m_pressedPos;
    bool m_bMoving;
};

#endif // MAINWINDOW_H
