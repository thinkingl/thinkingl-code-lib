#include <windows.h>
#include <process.h>
#include "picsave.h"
#include <string>
#include <QDir>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <Qtimer>

using namespace std;

picsave::picsave(QWidget *parent)
	: QMainWindow(parent)
	, m_pDownloader(0)
	, m_pTimer(0)
{
	ui.setupUi(this);

	this->setWindowIcon(QIcon(":/picsave/pic.ico"));

	// 读取配置.
	ReadConfig();

	// 按钮的信号绑定.
	connect(ui.pushButtonOK, SIGNAL(clicked()), SLOT(OnBtnOk()));	// OK
	connect(ui.pushButtonCancel, SIGNAL(clicked()), SLOT(OnBtnCancel()));	// 取消.
	connect(ui.pushButtonChooseNewDir, SIGNAL(clicked()), SLOT(OnBtnChooseNewDir()));	// 选择新目录.
	connect(ui.pushButtonOpenDir, SIGNAL(clicked()), SLOT(OnBtnOpenDir()));	// 打开目录.

	// 系统托盘.
	QAction* pActionShow = new QAction(this);
	pActionShow->setText(tr("显示"));

	QAction* pActionExit = new QAction(this);
	pActionExit->setText( tr("关闭") );

	QMenu* pTrayMenu = new QMenu(this);
	pTrayMenu->addAction(pActionShow);
	pTrayMenu->addAction(pActionExit);



	// 创建托盘.
	QSystemTrayIcon* pTray = new QSystemTrayIcon(this);

	pTray->setToolTip(tr("图片抓拍助手"));
	pTray->setContextMenu(pTrayMenu);
	pTray->setIcon(QIcon(":/picsave/pic.ico"));

	pTray->show();
	pTray->showMessage(tr("图片抓拍助手"),tr("图片抓拍"));

	// 信号连接.
	connect(pActionShow, SIGNAL(triggered(bool)), this, SLOT(OnTrayShow()));
	connect(pActionExit, SIGNAL(triggered(bool)), this, SLOT(OnTrayExit()));

	//点击托盘执行的事件
	connect(pTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
	connect(pTrayMenu, SIGNAL(showWidget()), this, SLOT(showNormal()));


	// 定时器.
	int secs = m_cfg.GetElapse();
	m_pTimer = new QTimer(this);
	connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnCheckPicTimer()));
	m_pTimer->setInterval(60 * 1000);	// 1分钟检查一次.
	m_pTimer->start();

	// 立马检查一次.
	emit OnCheckPicTimer();
}

void picsave::iconIsActived(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		//点击托盘显示窗口
	case QSystemTrayIcon::Trigger:
	{
		showNormal();
		break;
	}
		//双击托盘显示窗口
	case QSystemTrayIcon::DoubleClick:
	{
		showNormal();
		break;
	}
	default:
		break;
	}
}

picsave::~picsave()
{
	m_pTimer->stop();
}

void picsave::OnBtnOk()
{
	// 保存配置.

	// 服务器地址.
	QString addr = ui.lineEditServerAddr->text();
	m_cfg.SetServerAddr(addr);

	// 图片保存目录.
	QString dir = ui.lineEditPicDir->text();
	m_cfg.SetPicSaveDir(dir);

	// 抓拍间隔
	int elapseMin = ui.lineEditElapse->text().toInt();
	m_cfg.SetElapse(elapseMin * 60);

	// 开机自动运行.
	m_cfg;

	// 关闭.
	this->hide();
}

void picsave::OnBtnCancel()
{
	this->hide();
}

void picsave::OnBtnChooseNewDir()
{
	QString oldDir = ui.lineEditPicDir->text();
	QString newDir = QFileDialog::getExistingDirectory(this, tr("选择新的图片目录"), oldDir);
	if ( !newDir.isEmpty() )
	{
		ui.lineEditPicDir->setText(newDir);
	}
}

void picsave::OnBtnOpenDir()
{
	QString dir = m_cfg.GetPicSaveDir();

	dir.replace('/', '\\');

	QDir().mkpath(dir);

	wstring cmd = L" /e, ";
	cmd += dir.toStdWString();
	ShellExecute(
		NULL,
		(L"open"),
		//NULL,
		(L"Explorer.exe"),
		cmd.c_str(), // (L"/e, d:\\pics"),
		NULL,
		SW_SHOWDEFAULT);
}

void picsave::ReadConfig()
{
	// 服务器地址.
	QString addr = m_cfg.GetServerAddr();
	ui.lineEditServerAddr->setText(addr);

	// 图片保存目录.
	QString dir = m_cfg.GetPicSaveDir();
	ui.lineEditPicDir->setText(dir);

	// 抓拍间隔.
	int elapse = m_cfg.GetElapse();
	ui.lineEditElapse->setText(QString::number(elapse/60));

	// 开机自动运行.

}

void picsave::OnTrayShow()
{
	this->show();
}

void picsave::OnTrayExit()
{
	exit(0);
}

void picsave::closeEvent(QCloseEvent *event)
{
	this->hide();
	event->ignore();
}

void picsave::OnCheckPicTimer()
{
	// 检测xml文件有没有更新.

	// 下载xml设备信息文件.


	// 获取最近一次的图片下载时间.

	// 检查是否达到设置的间隔.

	// 开始下载图片.

}
