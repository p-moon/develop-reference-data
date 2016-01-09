#include "chatWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <QKeyEvent>

ChatWidget::ChatWidget(QWidget *parent)
    :QWidget(parent), m_recorder(this)
    , m_bRecorderValid(true)
    , m_bRecordSuccess(true)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch(1);

    m_stateLabel = new QLabel;
    layout->addWidget(m_stateLabel);

    QHBoxLayout *actionLayout = new QHBoxLayout();
    layout->addLayout(actionLayout);

    m_talkButton = new QPushButton("按住说话");
    connect(m_talkButton, SIGNAL(pressed()),
            this, SLOT(onTalkPressed()));
    connect(m_talkButton, SIGNAL(released()),
            this, SLOT(onTalkReleased()));
    actionLayout->addWidget(m_talkButton);

    m_playButton = new QPushButton("播放");
    connect(m_playButton, SIGNAL(clicked()),
            this, SLOT(onPlayButton()));
    actionLayout->addWidget(m_playButton);

    m_bRecorderValid = setupAudioRecorder();
    if(!m_bRecorderValid)
    {
        m_playButton->setEnabled(false);
        m_talkButton->setEnabled(false);
        m_stateLabel->setText("录音设备故障");
    }
    setupAudioPlayer();
}

ChatWidget::~ChatWidget()
{
}

bool ChatWidget::eventFilter(QObject *watched, QEvent *e)
{
    QEvent::Type t = e->type();
    switch(t)
    {
    case QEvent::KeyPress:
        if((static_cast<QKeyEvent *>(e))->key() == Qt::Key_Back)
        {
            e->accept();
            return true;
        }
        break;
    case QEvent::KeyRelease:
        if((static_cast<QKeyEvent *>(e))->key() == Qt::Key_Back)
        {
            QCoreApplication::quit();
            return true;
        }
        break;
    default:
        break;
    }
    return QWidget::eventFilter(watched, e);
}

void ChatWidget::onTalkPressed()
{
    if(m_bRecorderValid && m_recorder.state() == QMediaRecorder::StoppedState)
    {
        qDebug() << "start record";
        m_playButton->setEnabled(false);
        m_bRecordSuccess = true;
        m_filePath = QString("%1%2.%3")
                .arg(QDir::currentPath())
                .arg(QDateTime::currentDateTime().toString("yyMMdd-hhmmss"))
                .arg(m_recorder.containerFormat());
        m_recorder.setOutputLocation(QUrl::fromLocalFile(m_filePath));
        m_recorder.record();
    }
}

void ChatWidget::onTalkReleased()
{
    m_recorder.stop();
    QFile::setPermissions(m_filePath, QFile::ReadOwner | QFile::WriteOwner | QFile::ReadOther | QFile::ReadUser | QFile::ReadGroup);
    m_playButton->setEnabled(true);
}

void ChatWidget::onPlayButton()
{
    if(!m_bRecordSuccess) return;
    m_player->setMedia(QMediaContent(QUrl::fromLocalFile(m_filePath)));
    m_player->play();
}

void ChatWidget::onPlayStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch(status)
    {
    case QMediaPlayer::EndOfMedia:
        break;
    case QMediaPlayer::InvalidMedia:
        m_stateLabel->setText("invalidMedia");
        m_talkButton->setEnabled(true);
        m_playButton->setEnabled(true);
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

void ChatWidget::onPlayError(QMediaPlayer::Error error)
{
    qDebug() << "play error - " << error;
    m_talkButton->setEnabled(true);
    m_playButton->setEnabled(true);
}

void ChatWidget::onPlayStateChanged(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::StoppedState)
    {
        m_talkButton->setEnabled(true);
        m_playButton->setEnabled(true);
    }
}

void ChatWidget::onRecordDurationChanged(qint64 duration)
{
    int seconds = duration/1000;
    int ms = duration%1000;
    QString info = QString("已录制: %1 秒 %2 毫秒")
            .arg(seconds).arg(ms);
    m_stateLabel->setText(info);
}

void ChatWidget::onRecordStatusChanged(QMediaRecorder::Status)
{

}

void ChatWidget::onRecordError(QMediaRecorder::Error err)
{
    qDebug() << "record error - " << err;
    m_bRecordSuccess = false;
    m_playButton->setEnabled(true);
}


bool ChatWidget::setupAudioRecorder()
{
    //inputs
    QStringList inputs = m_recorder.audioInputs();
    if(inputs.size() == 0) return false;
    qDebug() << "inputs: " << inputs;
    m_recorder.setAudioInput("default");

    //audio codecs;
    QStringList codecs = m_recorder.supportedAudioCodecs();
    if(codecs.size() == 0) return false;
    qDebug() << "codecs: " << codecs;
    int sampleRate = 16000;
    if(codecs.contains("aac"))
    {
        m_settings.setCodec("aac");
    }
    else if(codecs.contains("amr-wb"))
    {
        m_settings.setCodec("amr-wb");
    }
    else
    {
        m_settings.setCodec(codecs.at(0));
        sampleRate = 8000;
    }
    qDebug() << "set codec: " << m_settings.codec();

    //containers
    QStringList containers = m_recorder.supportedContainers();
    if(containers.size() == 0) return false;
    qDebug() << "containers: " << containers;
    QString container;
    if(containers.contains("3gp"))
    {
        container = "3gp";
    }
    else if(containers.contains("mp4"))
    {
        container = "mp4";
    }
    else
    {
        container = containers.at(0);
    }

    //sample rate
    QList<int> sampleRates = m_recorder.supportedAudioSampleRates();
    if(sampleRates.size() == 0) return false;
    qDebug() << "samplerates: " << sampleRates;
    if(sampleRates.size() && !sampleRates.contains(sampleRate))
    {
        sampleRate = sampleRates.at(0);
    }

    m_settings.setChannelCount(2);
    m_settings.setSampleRate(sampleRate);
    m_settings.setQuality(QMultimedia::NormalQuality);
    m_settings.setBitRate(64000);
    m_settings.setEncodingMode(QMultimedia::AverageBitRateEncoding);
    m_recorder.setEncodingSettings(m_settings
                                   , QVideoEncoderSettings()
                                   , container);

    connect(&m_recorder, SIGNAL(durationChanged(qint64)), this,
            SLOT(onRecordDurationChanged(qint64)));
    connect(&m_recorder, SIGNAL(statusChanged(QMediaRecorder::Status)), this,
            SLOT(onRecordStatusChanged(QMediaRecorder::Status)));
    connect(&m_recorder, SIGNAL(error(QMediaRecorder::Error)), this,
            SLOT(onRecordError(QMediaRecorder::Error)));

    return true;
}

void ChatWidget::setupAudioPlayer()
{
    m_player = new QMediaPlayer(this);
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(onPlayStatusChanged(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(error(QMediaPlayer::Error)),
            this, SLOT(onPlayError(QMediaPlayer::Error)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(onPlayStateChanged(QMediaPlayer::State)));
}
