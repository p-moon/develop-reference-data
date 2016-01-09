#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMediaPlayer>

class SimpleAudioPlayer : public QObject
{
    Q_OBJECT
public:
    SimpleAudioPlayer();
    ~SimpleAudioPlayer();

    void play(const QString &filePath);
    void stop();

signals:
    void playError();
    void playFinished();

protected slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onError(QMediaPlayer::Error error);
    void onStateChanged(QMediaPlayer::State state);

private:
    QMediaPlayer *m_player;
};

#endif // AUDIOPLAYER_H
