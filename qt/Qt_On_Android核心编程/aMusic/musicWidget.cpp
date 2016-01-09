#include "musicWidget.h"
#include <QApplication>
#include <QScreen>
#include <QSize>
#include <QResizeEvent>
#include <QUrl>
#include <QSensorGestureManager>
#include <QDebug>
#include <QVBoxLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QPaintEvent>
#include <QPainter>
#include "rowDelegate.h"

static const char * g_playmodeTexts[] = {
    "单曲播放","单曲循环","顺序播放","列表循环","随机播放"
};

#define SLIDER_STYLE \
  "QSlider::groove:horizontal {"\
  "    border: 1px solid #999999;"\
  "    height: 10px; "\
  "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #383a3f80, STOP:0.5 #1c1e2380, stop:1 #383a3f80);"\
  "    margin: 2px 0;"\
  "    border-radius: 4px;"\
  "}"\
  "QSlider::handle:horizontal {"\
  "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #d5AA30, stop:0.5 #ff9A02, stop:1 #D5AA30);"\
  "    border: 1px solid #5c5c5c;"\
  "    width: 8px;"\
  "    margin: -2px 0;" \
  "    border-radius: 3px;"\
  "}"\
  "QSlider::sub-page:horizontal {"\
  "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #dc9902, stop:0.5 #E79A02,stop:1 #dc9902);"\
  "    margin: 4px 3;"\
  "}"

#define MUTE_CHECKBOX_STYLE \
    "QCheckBox {"\
    "    max-width: 32px;"\
    "    min-width: 32px;"\
    "    max-height:32px;"\
    "    min-height:32px;"\
    "    border: 0px;"\
    "    padding: 0px;"\
    "    margin: 0px;"\
    "}"\
    "QCheckBox::indicator {"\
    "    width: 32px;"\
    "    height:32px;"\
    "}"\
    "QCheckBox::indicator:unchecked {"\
    "    image: url(:/images/songVolume.png);"\
    "}"\
    "QCheckBox::indicator:checked {"\
    "    image: url(:/images/songVolume_quiet.png);"\
    "}"

#define CURRENT_LABEL_STYLE \
    "QLabel {"\
    "  background-color: #00000000;"\
    "  color: #E0E0E0;"\
    "  border: 1px solid;"\
    "  border-color: white white #8f8f91 red;"\
    "  border-width: 0px 0px 1px 0px;"\
    "  margin-left: 1px;"\
    "  margin-left: 1px;"\
    "  margin-top: 1px;"\
    "  margin-bottom: 3px;"\
    "}"

#define PLAYLIST_VIEW_STYLE \
    "QTableView {"\
    "  background: #00000000;"\
    "  border: 0px;"\
    "  selection-background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #F59A20F0, stop: 0.4 #EA8B7180,stop: 1 #05030600);"\
    "}"\
    "QAbstractScrollArea {"\
    "background: #00000000;"\
    "}"\
    "QHeaderView::section {"\
    "background: #00000000;"\
    "color: white;"\
    "padding-left: 4px;"\
    "padding-right: 4px;"\
    "border: 0px;"\
    "}"

MusicWidget::MusicWidget(QWidget *parent)
    : QWidget(parent)
    , m_player(0), m_playlist(0)
    , m_shakeGesture(0), m_turnoverGesture(0)
    , m_bSliderCanUpdate(true)
    , m_bkgndLogo(":/images/bkgnd.png")
    , m_nAutoNextTimer(0)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    initPlayer();
    setupSensors();
    setupGUI();
}

MusicWidget::~MusicWidget()
{

}

void MusicWidget::playFile(QString strFile, QString strPath
                             , QStringList &files)
{
    int fileCount = files.size();
    m_playlistView->clear();
    m_playlistView->setRowCount(fileCount);
    m_playlistView->setColumnCount(1);
    m_playlistView->setColumnWidth(0, m_playlistView->width() - 50);
    m_playlistView->horizontalHeader()->setVisible(false);

    m_player->stop();
    m_playlist->clear();

    for(int i = 0; i < fileCount; i++)
    {
        const QString &fileName = files.at(i);
        m_playlist->addMedia(QUrl::fromLocalFile(strPath + "/" + fileName));
        m_playlistView->setItem(i, 0, new QTableWidgetItem(fileName));
    }
    int position = files.indexOf(strFile);
    m_playlistView->selectRow(position);
    m_playlist->setCurrentIndex(position);
    m_player->play();
    m_currentSongLabel->setText(strFile);

    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_playModeButton->setText(g_playmodeTexts[QMediaPlaylist::Loop]);
    if(m_playlist->mediaCount() > 1)
    {
        if(m_shakeGesture)m_shakeGesture->startDetection();
        if(m_turnoverGesture)m_turnoverGesture->startDetection();
    }
}

void MusicWidget::onTurnover()
{
    if(m_player && m_player->state() == QMediaPlayer::PlayingState)
    {
        m_player->pause();
    }
}

void MusicWidget::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch(status)
    {
    case QMediaPlayer::EndOfMedia:
        break;
    case QMediaPlayer::InvalidMedia:
        //TODO: error report
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

void MusicWidget::onCurrentMediaIndexChanged(int pos)
{
    m_playlistView->selectRow(pos);
    m_currentSongLabel->setText(m_playlist->media(pos).canonicalUrl().fileName());
}

void MusicWidget::onError(QMediaPlayer::Error error)
{
    //TODO
}

void MusicWidget::onDurationChanged(qint64 duration)
{
    m_playProgress->setRange(0, duration);
    int seconds = duration / 1000;
    QString text;
    text.sprintf("%02d:%02d", seconds/60, seconds%60);
    m_durationLabel->setText(text);
}

void MusicWidget::onPositionChanged(qint64 position)
{
    if(m_bSliderCanUpdate /* !m_playProgress->isSliderDown()*/)
        m_playProgress->setValue(position);
    int seconds = position / 1000;
    QString text;
    text.sprintf("%02d:%02d", seconds/60, seconds%60);
    m_positionLabel->setText(text);
}

void MusicWidget::onSeekableChanged(bool seekable)
{
    qDebug() << "current media seekable - " << seekable;
}

void MusicWidget::onStateChanged(QMediaPlayer::State state)
{
    qDebug() << "playstate changed - " << state;
#ifdef ANDROID
    if(state == QMediaPlayer::StoppedState)
    {
        if(m_nAutoNextTimer > 0)
        {
            killTimer(m_nAutoNextTimer);
        }
        m_nAutoNextTimer = startTimer(0);
    }
#endif
}

void MusicWidget::onPlayPauseButton()
{
    if(m_player)
    {
        switch(m_player->state())
        {
        case QMediaPlayer::PlayingState:
            m_player->pause();
            break;
        case QMediaPlayer::PausedState:
        case QMediaPlayer::StoppedState:
            m_player->play();
            break;
        }
    }
}

void MusicWidget::onStopButton()
{
    if(m_player)
    {
        m_player->stop();
    }
}

void MusicWidget::onSliderPressed()
{
    m_bSliderCanUpdate = false;
}

void MusicWidget::onSliderReleased()
{
    m_bSliderCanUpdate = true;
    if(m_player)
    {
        m_player->setPosition(m_playProgress->sliderPosition());
    }
}

void MusicWidget::onCellClicked(int row, int column)
{
    if(column == 0)
    {
        gotoSong(row);
    }
}

void MusicWidget::onMuteButtonStateChanged(int state)
{
    switch(state)
    {
    case Qt::Checked:
        if(m_player && !m_player->isMuted()) m_player->setMuted(true);
        break;
    case Qt::Unchecked:
        if(m_player && m_player->isMuted()) m_player->setMuted(false);
        break;
    }
}

void MusicWidget::onPlayModeButton()
{
    if(!m_playlist) return;
    int mode = m_playlist->playbackMode() + 1;
    if( mode > QMediaPlaylist::Random )
        mode = QMediaPlaylist::CurrentItemOnce;
    m_playlist->setPlaybackMode((QMediaPlaylist::PlaybackMode)mode);
    m_playModeButton->setText(g_playmodeTexts[mode]);
}

void MusicWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    QRect srcRect = m_bkgndLogo.rect();
    QRect destRect(srcRect);
    int xpos = width() - m_bkgndLogo.width() - 30;
    int ypos = m_currentSongLabel->y() - m_bkgndLogo.height() - 10;
    destRect.moveTopLeft(QPoint(xpos, ypos));
    p.drawPixmap(destRect, m_bkgndLogo);
}

void MusicWidget::timerEvent(QTimerEvent *e)
{
    if(m_nAutoNextTimer == e->timerId())
    {
        int nextIndex = -1;
        switch(m_playlist->playbackMode())
        {
        case QMediaPlaylist::CurrentItemOnce:
            break;
        case QMediaPlaylist::CurrentItemInLoop:
            nextIndex = m_playlist->currentIndex();
            break;
        case QMediaPlaylist::Sequential:
        case QMediaPlaylist::Loop:
        case QMediaPlaylist::Random:
            nextIndex = m_playlist->nextIndex();
            break;
        }
        if(nextIndex >= 0)
        {
            m_playlist->setCurrentIndex(nextIndex);
            m_player->play();
        }
        killTimer(m_nAutoNextTimer);
        m_nAutoNextTimer = 0;
    }
    else
    {
        QWidget::timerEvent(e);
    }
}

void MusicWidget::setupGUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addSpacing(10);

    m_playlistView = new QTableWidget();
    RowDelegate *rowDelegate = new RowDelegate(this);
    m_playlistView->setItemDelegate(rowDelegate);
    m_playlistView->setStyleSheet(PLAYLIST_VIEW_STYLE);
    m_playlistView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_playlistView->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    m_playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_playlistView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_playlistView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_playlistView->setShowGrid(false);
    layout->addWidget(m_playlistView);
    connect(m_playlistView, SIGNAL(cellClicked(int,int))
            , this, SLOT(onCellClicked(int,int)));
    layout->addSpacing(4);

    m_currentSongLabel = new QLabel();
    m_currentSongLabel->setStyleSheet(CURRENT_LABEL_STYLE);
    layout->addWidget(m_currentSongLabel);

    QSize buttonSize(41, 33);
    m_playModeButton = new QPushButton(g_playmodeTexts[QMediaPlaylist::Loop], this);
    m_playModeButton->adjustSize();
#ifdef ANDROID
    QSize screenSize = qApp->primaryScreen()->size();
    if(screenSize.width() > 960 || screenSize.height() > 960)
    {
        buttonSize *= (m_playModeButton->height() / 33);
    }
#endif
    QHBoxLayout * controlLayout = new QHBoxLayout;
    controlLayout->setSpacing(4);
    layout->addLayout(controlLayout);
    controlLayout->addStretch(1);
    m_folderButton = new ImageButton(QPixmap(":/images/openfile.png")
                                     , QPixmap(":/images/openfile_down.png"));
    connect(m_folderButton, SIGNAL(clicked()), this, SIGNAL(selectSong()));
    m_folderButton->setFixedSize(buttonSize);
    controlLayout->addWidget(m_folderButton);

    m_prevButton = new ImageButton(QPixmap(":/images/preSong.png"),
                                   QPixmap(":/images/preSong_down.png"));
    m_prevButton->setFixedSize(buttonSize);
    connect(m_prevButton, SIGNAL(clicked()), m_playlist, SLOT(previous()));
    controlLayout->addWidget(m_prevButton);

    m_playpauseButton = new ImageButton(QPixmap(":/images/playpause.png")
                                        ,QPixmap(":/images/playpause_down.png"));
    m_playpauseButton->setFixedSize(buttonSize);
    connect(m_playpauseButton, SIGNAL(clicked()), this, SLOT(onPlayPauseButton()));
    controlLayout->addWidget(m_playpauseButton);

    m_stopButton = new ImageButton(QPixmap(":/images/stop.png")
                                   , QPixmap(":/images/stop_down.png"));
    m_stopButton->setFixedSize(buttonSize);
    connect(m_stopButton, SIGNAL(clicked()), this, SLOT(onStopButton()));
    controlLayout->addWidget(m_stopButton);

    m_nextButton = new ImageButton(QPixmap(":/images/nextSong.png")
                                   , QPixmap(":/images/nextSong_down.png"));
    m_nextButton->setFixedSize(buttonSize);
    connect(m_nextButton, SIGNAL(clicked()), m_playlist, SLOT(next()));
    controlLayout->addWidget(m_nextButton);

    QCheckBox *muteBox = new QCheckBox();
    muteBox->setStyleSheet(MUTE_CHECKBOX_STYLE);
    connect(muteBox, SIGNAL(stateChanged(int)),
            this, SLOT(onMuteButtonStateChanged(int)));
    controlLayout->addWidget(muteBox);

    connect(m_playModeButton, SIGNAL(clicked()),
            this, SLOT(onPlayModeButton()));
    controlLayout->addWidget(m_playModeButton);

    controlLayout->addStretch(1);

    QGridLayout *progressLayout = new QGridLayout();
    layout->addLayout(progressLayout);
    m_playProgress = new QSlider(Qt::Horizontal);
    //m_playProgress->setFixedWidth(280);
    m_playProgress->setStyleSheet(SLIDER_STYLE);
    progressLayout->addWidget(m_playProgress, 0, 0, 1, 2);
    connect(m_playProgress, SIGNAL(sliderPressed()),
            this, SLOT(onSliderPressed()));
    connect(m_playProgress, SIGNAL(sliderReleased()),
            this, SLOT(onSliderReleased()));
    m_positionLabel = new QLabel();
    m_positionLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    progressLayout->addWidget(m_positionLabel, 1, 0, Qt::AlignLeft | Qt::AlignTop);
    m_durationLabel = new QLabel();
    m_durationLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    progressLayout->addWidget(m_durationLabel, 1, 1, Qt::AlignRight | Qt::AlignTop);
}

void MusicWidget::initPlayer()
{
    m_player = new QMediaPlayer(this);
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus))
            ,this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(onStateChanged(QMediaPlayer::State)));
    connect(m_player, SIGNAL(seekableChanged(bool)),
            this, SLOT(onSeekableChanged(bool)));
    connect(m_player, SIGNAL(error(QMediaPlayer::Error)),
            this, SLOT(onError(QMediaPlayer::Error)));
    connect(m_player, SIGNAL(durationChanged(qint64)),
            this, SLOT(onDurationChanged(qint64)));
    connect(m_player, SIGNAL(positionChanged(qint64)),
            this, SLOT(onPositionChanged(qint64)));

    m_playlist = new QMediaPlaylist(this);
    connect(m_playlist, SIGNAL(currentIndexChanged(int)),
            this, SLOT(onCurrentMediaIndexChanged(int)));
    m_player->setPlaylist(m_playlist);

}

void MusicWidget::setupSensors()
{
    QSensorGestureManager manager;
    QStringList gestureIds = manager.gestureIds();
    qDebug() << gestureIds;
    if(gestureIds.contains("QtSensors.shake2"))
    {
        qDebug() << "setup shake 2 sensor";
        m_shakeGesture = new QSensorGesture(
                    QStringList() << "QtSensors.shake2"
                    , this);
        connect(m_shakeGesture, SIGNAL(shakeUp()),
                m_playlist, SLOT(previous()));
        connect(m_shakeGesture, SIGNAL(shakeDown()),
                m_playlist, SLOT(next()));
        connect(m_shakeGesture, SIGNAL(shakeLeft()),
                m_playlist, SLOT(previous()));
        connect(m_shakeGesture, SIGNAL(shakeRight()),
                m_playlist, SLOT(next()));

    }
    else if(gestureIds.contains("QtSensors.shake"))
    {
        qDebug() << "setup shake sensor";
        m_shakeGesture = new QSensorGesture(
                    QStringList() << "QtSensors.shake"
                    , this);
        connect(m_shakeGesture, SIGNAL(shake()),
                m_playlist, SLOT(next()));
    }
    if(gestureIds.contains("QtSensors.turnover"))
    {
        m_turnoverGesture = new QSensorGesture(
                    QStringList("QtSensors.turnover")
                    ,this
                    );
        connect(m_turnoverGesture, SIGNAL(turnover()),
                this, SLOT(onTurnover()));
    }
}

void MusicWidget::gotoSong(int index)
{
    if(!m_playlist) return;
    m_playlist->setCurrentIndex(index);
    m_player->play();

    m_currentSongLabel->setText(m_playlist->media(index).canonicalUrl().fileName());
}

