#include "audioplayer.h"
#include <QDebug>

SimpleAudioPlayer::SimpleAudioPlayer()
{
    m_player = new QMediaPlayer(this);
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus))
            ,this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(onStateChanged(QMediaPlayer::State)));
    connect(m_player, SIGNAL(error(QMediaPlayer::Error)),
            this, SLOT(onError(QMediaPlayer::Error)));
}

SimpleAudioPlayer::~SimpleAudioPlayer()
{

}

void SimpleAudioPlayer::play(const QString &filePath)
{
    qDebug() << "try to play - " << filePath;
    stop();
    m_player->setMedia(QMediaContent(QUrl(filePath)));
    m_player->play();
}

void SimpleAudioPlayer::stop()
{
    m_player->stop();
}

void SimpleAudioPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch(status)
    {
    case QMediaPlayer::EndOfMedia:
        qDebug() << "EndOfMedia";
        emit playFinished();
        break;
    case QMediaPlayer::InvalidMedia:
        qDebug() << "invalidMedia";
        emit playError();
        break;
    case QMediaPlayer::BufferingMedia:
        break;
    case QMediaPlayer::BufferedMedia:
        break;
    case QMediaPlayer::LoadingMedia:
        break;
    case QMediaPlayer::LoadedMedia:
        break;
    }
}

void SimpleAudioPlayer::onError(QMediaPlayer::Error error)
{
    qDebug() << "play error " << error;
    emit playError();
}

void SimpleAudioPlayer::onStateChanged(QMediaPlayer::State state)
{

}

