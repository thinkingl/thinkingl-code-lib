#include "xplayerqt.h"

#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QFileDialog>

XPlayerQt::XPlayerQt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	InitPlayer();

	ConnectSignals();
}

XPlayerQt::~XPlayerQt()
{

}

void XPlayerQt::InitPlayer()
{
	m_pPlaylist = new QMediaPlaylist(this);
	m_pPlayer = new QMediaPlayer(this);
	m_pPlayer->setPlaylist(m_pPlaylist);

	connect(m_pPlayer, SIGNAL(audioAvailableChanged(bool)), this, SLOT(OnMediaPlayerAudioAvailableChanged(bool)));
	connect(m_pPlayer, SIGNAL(bufferStatusChanged(int)), this, SLOT(OnMediaPlayerBufferStatusChanged(int)));
	connect(m_pPlayer, SIGNAL(currentMediaChanged(const QMediaContent&)), this, SLOT(OnMediaPlayerCurrentMediaChanged(const QMediaContent&)));
	connect(m_pPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(OnMediaPlayerDurationChanged(qint64)));
	connect(m_pPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(OnMediaPlayerError(QMediaPlayer::Error)));
	connect(m_pPlayer, SIGNAL(mediaChanged(const QMediaContent&)), this, SLOT(OnMediaPlayerMediaChanged(const QMediaContent&)));
	connect(m_pPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(OnMediaPlayerMediaStatusChanged(QMediaPlayer::MediaStatus)));
	connect(m_pPlayer, SIGNAL(mutedChanged(bool)), this, SLOT(OnMediaPlayerMutedChanged(bool)));
	connect(m_pPlayer, SIGNAL(networkConfigurationChanged(const QNetworkConfiguration&)), this, SLOT(OnMediaPlayerNetworkConfigurationChanged(const QNetworkConfigration&)));
	connect(m_pPlayer, SIGNAL(playbackRateChanged(qreal)), this, SLOT(OnMediaPlayerPlaybackRateChanged(qreal)));
	connect(m_pPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(OnMediaPlayerPositionChanged(qint64)));
	connect(m_pPlayer, SIGNAL(seekableChanged(bool)), this, SLOT(OnMediaPlayerSeekableChanged(bool)));
	connect(m_pPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(OnMediaPlayerStateChanged(QMediaPlayer::State)));
	connect(m_pPlayer, SIGNAL(videoAvailiableChanged(bool)), this, SLOT(OnMediaPlayerVideoAvailableChanged(bool)));
	connect(m_pPlayer, SIGNAL(volumeChanged(int)), this, SLOT(OnMediaPlayerVolumeChanged(int)));

	m_pVideoWidget = new QVideoWidget(this);
	
	// 放到应当的位置.
	ui.horizontalLayoutVideo->addWidget( m_pVideoWidget );

	QSizePolicy cur = m_pVideoWidget->sizePolicy(); // (QSizePolicy)
	m_pPlayer->setVideoOutput(m_pVideoWidget);



	m_pVideoWidget->show();
}

void XPlayerQt::ConnectSignals()
{
	connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(OnOpenFile()));
	connect(ui.pushButtonPlay, SIGNAL(clicked()), this, SLOT(OnOpenFile()));
	// ui.mainToolBar;
}

void XPlayerQt::OnOpenFile()
{
	QUrl filePath = QFileDialog::getOpenFileUrl(this, tr("Open meida file"));
	if ( !filePath.isEmpty() )
	{
		m_pPlaylist->addMedia( filePath);
		m_pPlaylist->setCurrentIndex(m_pPlaylist->mediaCount() - 1);
		m_pPlayer->play();
	}
}

void	XPlayerQt::OnMediaPlayerAudioAvailableChanged(bool available)
{
	qDebug() << "Media player audio available changed, available: " << available;
}
void	XPlayerQt::OnMediaPlayerBufferStatusChanged(int percentFilled)
{
	qDebug() << "Media player buffer status changed, percent filled: " << percentFilled;
}
void	XPlayerQt::OnMediaPlayerCurrentMediaChanged(const QMediaContent & media)
{
	qDebug() << "Media player current media changed, media content: " << media.canonicalResource().url();
}
void	XPlayerQt::OnMediaPlayerDurationChanged(qint64 duration)
{
	qDebug() << "Media player duration changed, duration: " << duration;
}
void	XPlayerQt::OnMediaPlayerError(QMediaPlayer::Error error)
{
	qDebug() << "Media player error! er: " << error;
}
void	XPlayerQt::OnMediaPlayerMediaChanged(const QMediaContent & media)
{
	qDebug() << "Media player media changed, media content: " << media.canonicalResource().url();
}
void	XPlayerQt::OnMediaPlayerMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
	qDebug() << "Media player media status changed, status: " << status;
}
void	XPlayerQt::OnMediaPlayerMutedChanged(bool muted)
{
	qDebug() << "Media player mute changed, muted: " << muted;
}
void	XPlayerQt::OnMediaPlayerNetworkConfigurationChanged(const QNetworkConfiguration & configuration)
{
	qDebug() << "Media player network configuration changed, configuration: " << configuration.name();
}
void	XPlayerQt::OnMediaPlayerPlaybackRateChanged(qreal rate)
{
	qDebug() << "Media player playback rate changed, rate: " << rate;
}
void	XPlayerQt::OnMediaPlayerPositionChanged(qint64 position)
{
	qDebug() << "Media player position changed, position: " << position;
}
void	XPlayerQt::OnMediaPlayerSeekableChanged(bool seekable)
{
	qDebug() << "Media player seekable changed, seekable: " << seekable;
}
void	XPlayerQt::OnMediaPlayerStateChanged(QMediaPlayer::State state)
{
	qDebug() << "Media player state changed, state: " << state;
}
void	XPlayerQt::OnMediaPlayerVideoAvailableChanged(bool videoAvailable)
{
	qDebug() << "Media player video available changed, video available: " << videoAvailable;
}
void	XPlayerQt::OnMediaPlayerVolumeChanged(int volume)
{
	qDebug() << "Media player volume changed, volume: " << volume;
}