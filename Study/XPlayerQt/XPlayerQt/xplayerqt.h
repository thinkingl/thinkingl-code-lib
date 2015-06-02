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

private:
	Ui::XPlayerQtClass ui;

private:
	QMediaPlayer* m_pPlayer;
	QMediaPlaylist* m_pPlaylist;
	QVideoWidget* m_pVideoWidget;
};

#endif // XPLAYERQT_H
