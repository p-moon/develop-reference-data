#ifndef WIDGET_H
#define WIDGET_H
#include "../openFileWidget.h"
#include "musicWidget.h"
#include <QStackedLayout>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
#ifndef ANDROID
    QSize sizeHint() const { return QSize(320,480); }
#endif

protected slots:
    void onSelectSong();
    void onFileSelected(QString strFile);
    void onFileSelectCanceled();

protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *, QEvent *);

private:
    void setupGUI();

private:
    QStackedLayout *m_layout;
    OpenFileWidget * m_fileSelector;
    MusicWidget *m_player;
};

#endif // WIDGET_H
