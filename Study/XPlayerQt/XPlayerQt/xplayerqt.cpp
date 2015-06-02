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

// 	void	OnMediaPlayerNetworkConfigurationChanged(const QNetworkConfiguration & configuration);
// 	void	OnMediaPlayerPlaybackRateChanged(qreal rate);
// 	void	OnMediaPlayerPositionChanged(qint64 position);
// 	void	OnMediaPlayerSeekableChanged(bool seekable);
// 	void	OnMediaPlayerStateChanged(QMediaPlayer::State state);
// 	void	OnMediaPlayerVideoAvailableChanged(bool videoAvailable);
// 	void	OnMediaPlayerVolumeChanged(int volume);

	connect(m_pPlayer, SIGNAL(audioAvailableChanged(bool)), this, SIGNAL(OnMediaPlayerAudioAvailableChanged(bool)));
	connect(m_pPlayer, SIGNAL(bufferStatusChanged(int)), this, SIGNAL(OnMediaPlayerBufferStatusChanged(int)));
	connect(m_pPlayer, SIGNAL(currentMediaChanged(const QMediaContent&)), this, SIGNAL(OnMediaPlayerCurrentMediaChanged(const QMediaContent&)));
	connect(m_pPlayer, SIGNAL(durationChanged(qint64)), this, SIGNAL(OnMediaPlayerDurationChanged(qint64)));
	connect(m_pPlayer, SIGNAL(error(QMediaPlayer::Error)), this, SIGNAL(OnMediaPlayerError(QMediaPlayer::Error)));
	connect(m_pPlayer, SIGNAL(mediaChanged(const QMediaContent&)), this, SIGNAL(OnMediaPlayerMediaChanged(const QMediaContent&)));
	connect(m_pPlayer, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SIGNAL(OnMediaPlayerMediaStatusChanged(QMediaPlayer::MediaStatus)));
	connect(m_pPlayer, SIGNAL(mutedChanged(bool)), this, SIGNAL(OnMediaPlayerMutedChanged(bool)));
	connect(m_pPlayer, SIGNAL(networkConfigurationChanged(const QNetworkConfiguration&)), this, SIGNAL(OnMediaPlayerNetworkConfigurationChanged(const QNetworkConfigration&)));
	connect(m_pPlayer, SIGNAL(), this, SIGNAL(OnMediaPlayer());
	connect(m_pPlayer, SIGNAL(), this, SIGNAL(OnMediaPlayer());
	connect(m_pPlayer, SIGNAL(), this, SIGNAL(OnMediaPlayer());
	connect(m_pPlayer, SIGNAL(), this, SIGNAL(OnMediaPlayer());
	connect(m_pPlayer, SIGNAL(), this, SIGNAL(OnMediaPlayer());
	connect(m_pPlayer, SIGNAL(), this, SIGNAL(OnMediaPlayer());

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