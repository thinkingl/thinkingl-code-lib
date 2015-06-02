#ifndef XPLAYERQT_H
#define XPLAYERQT_H

#include <QtWidgets/QMainWindow>
#include "ui_xplayerqt.h"

#include <QMediaPlayer>

class XPlayerQt : public QMainWindow
{
	Q_OBJECT

public:
	XPlayerQt(QWidget *parent = 0);
	~XPlayerQt();

	// 初始化.
	void InitPlayer();

	// 连接信号.
	void ConnectSignals();

private slots:
	// 打开文件.
	void OnOpenFile();


	// QMediaPlayer的信号.
	void	OnMediaPlayerAudioAvailableChanged(bool available);
	void	OnMediaPlayerBufferStatusChanged(int percentFilled);
	void	OnMediaPlayerCurrentMediaChanged(const QMediaContent & media);
	void	OnMediaPlayerDurationChanged(qint64 duration);
	void	OnMediaPlayerError(QMediaPlayer::Error error);
	void	OnMediaPlayerMediaChanged(const QMediaContent & media);
	void	OnMediaPlayerMediaStatusChanged(QMediaPlayer::MediaStatus status);
	void	OnMediaPlayerMutedChanged(bool muted);
	void	OnMediaPlayerNetworkConfigurationChanged(const QNetworkConfiguration & configuration);
	void	OnMediaPlayerPlaybackRateChanged(qreal rate);
	void	OnMediaPlayerPositionChanged(qint64 position);
	void	OnMediaPlayerSeekableChanged(bool seekable);
	void	OnMediaPlayerStateChanged(QMediaPlayer::State state);
	void	OnMediaPlayerVideoAvailableChanged(bool videoAvailable);
	void	OnMediaPlayerVolumeChanged(int volume);

private:
	Ui::XPlayerQtClass ui;

private:
	QMediaPlayer* m_pPlayer;
	QMediaPlaylist* m_pPlaylist;
	QVideoWidget* m_pVideoWidget;
};

#endif // XPLAYERQT_H
