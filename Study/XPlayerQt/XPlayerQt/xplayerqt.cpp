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