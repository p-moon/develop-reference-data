#ifndef CHATWIDGET_H
#define CHATWIDGET_H
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    ChatWidget(QWidget *parent = 0);
    ~ChatWidget();

    QSize sizeHint() const{ return QSize(480, 800); }

protected:
    bool eventFilter(QObject *, QEvent *);

protected slots:
    void onTalkPressed();
    void onTalkReleased();
    void onPlayButton();
    void onPlayStatusChanged(QMediaPlayer::MediaStatus status);
    void onPlayError(QMediaPlayer::Error error);
    void onPlayStateChanged(QMediaPlayer::State state);
    void onRecordDurationChanged(qint64 duration);
    void onRecordStatusChanged(QMediaRecorder::Status);
    void onRecordError(QMediaRecorder::Error);

private:
    bool setupAudioRecorder();
    void setupAudioPlayer();

private:
    QPushButton *m_talkButton;
    QPushButton *m_playButton;
    QLabel *m_stateLabel;
    QAudioRecorder m_recorder;
    QAudioEncoderSettings m_settings;
    QMediaPlayer *m_player;
    QString m_filePath;
    bool m_bRecorderValid;
    bool m_bRecordSuccess;
};

#endif // CHATWIDGET_H
