#include "audiorecorder.h"
#include <QDateTime>
#include <QUrl>
#include <QFile>
/*
 * 05-28 13:54:39.281 27785 27847 D LanVoiceMessenger: inputs:  ("default", "mic", "voice_uplink", "voi
ce_downlink", "voice_call", "voice_recognition")
05-28 13:54:39.281 27785 27847 D LanVoiceMessenger: codecs:  ("amr-nb", "amr-wb", "aac")
05-28 13:54:39.281 27785 27847 D LanVoiceMessenger: containers:  ("mp4", "3gp", "amr", "awb")
05-28 13:54:39.291 27785 27847 D LanVoiceMessenger: samplerates:  (8000, 11025, 12000, 16000, 22050,
 24000, 32000, 44100, 48000, 96000)
 */
SimpleAudioRecorder::SimpleAudioRecorder(const QString &saveDir)
    : m_baseDir(saveDir), m_recorder(this), m_bValid(true)
    , m_duration(0), m_bSuccess(true)
{
    if(!m_baseDir.endsWith('/')) m_baseDir.append('/');
    m_bValid = setupRecorder();
    m_bSuccess = m_bValid;
}

SimpleAudioRecorder::~SimpleAudioRecorder()
{

}

void SimpleAudioRecorder::record()
{
    if(m_bValid && m_recorder.state() == QMediaRecorder::StoppedState)
    {
        m_duration = 0;
        m_bSuccess = true;

        m_filePath = QString("%1%2.%3")
                .arg(m_baseDir)
                .arg(QDateTime::currentDateTime().toString("yyMMdd-hhmmss"))
                .arg(m_recorder.containerFormat());
        m_recorder.setOutputLocation(QUrl::fromLocalFile(m_filePath));
        m_recorder.record();
    }
}

void SimpleAudioRecorder::stop()
{
    if(m_bValid)
    {
        m_recorder.stop();
        QFile::setPermissions(m_filePath, QFile::ReadOwner | QFile::WriteOwner | QFile::ReadOther | QFile::ReadUser | QFile::ReadGroup);
    }
}

QString SimpleAudioRecorder::currentFile()
{
    return m_filePath;
}

qint64 SimpleAudioRecorder::duration()
{
    return m_duration;
}

bool SimpleAudioRecorder::success()
{
    return m_bSuccess;
}

void SimpleAudioRecorder::onDurationChanged(qint64 duration)
{
    m_duration = duration;
}

void SimpleAudioRecorder::onStatusChanged(QMediaRecorder::Status)
{

}

void SimpleAudioRecorder::onError(QMediaRecorder::Error e)
{
    if(e != QMediaRecorder::NoError) m_bSuccess = false;
}

bool SimpleAudioRecorder::setupRecorder()
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
    if(containers.contains("3gp"))
    {
        m_container = "3gp";
    }
    else if(containers.contains("mp4"))
    {
        m_container = "mp4";
    }
    else
    {
        m_container = containers.at(0);
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
                                   , m_container);

    connect(&m_recorder, SIGNAL(durationChanged(qint64)), this,
            SLOT(onDurationChanged(qint64)));
    connect(&m_recorder, SIGNAL(statusChanged(QMediaRecorder::Status)), this,
            SLOT(onStatusChanged(QMediaRecorder::Status)));
    connect(&m_recorder, SIGNAL(error(QMediaRecorder::Error)), this,
            SLOT(onError(QMediaRecorder::Error)));

    return true;
}

