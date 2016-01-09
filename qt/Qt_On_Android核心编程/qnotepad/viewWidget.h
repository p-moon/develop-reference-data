#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include "note.h"
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include "../imageButton.h"

class NoteViewWidget : public QWidget{
    Q_OBJECT
public:
    NoteViewWidget(QList<QNote*> &notes,
                   QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~NoteViewWidget();

    void setNote(QNote *note);

signals:
    void notesUpdate();

protected slots:
    void onThumbnails();
    void onSave();
    void onDelete();
    void onAdd();
    void onTextChanged();

protected:
    void paintEvent(QPaintEvent * e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void setupGUI();
    void saveNotes();
    void checkChangeSave();

private:
    QNote *m_note;
    QList<QNote*> &m_notes;
    ImageButton *m_thumbnailsButton;
    ImageButton *m_saveButton;
    ImageButton *m_deleteButton;
    ImageButton *m_addButton;
    QTextEdit *m_edit;
    QLabel *m_titleLabel;
    bool m_bTextChanged;
};

#endif // VIEWWIDGET_H
