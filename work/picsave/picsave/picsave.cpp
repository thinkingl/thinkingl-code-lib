#include <windows.h>
#include <process.h>
#include "picsave.h"
#include <string>
#include <QDir>
#include <QFileDialog>

using namespace std;

picsave::picsave(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// 读取配置.
	ReadConfig();

	// 按钮的信号绑定.
	connect(ui.pushButtonOK, SIGNAL(clicked()), SLOT(OnBtnOK()));	// OK
	connect(ui.pushButtonCancel, SIGNAL(clicked()), SLOT(OnBtnCancel()));	// 取消.
	connect(ui.pushButtonChooseNewDir, SIGNAL(clicked()), SLOT(OnBtnChooseNewDir()));	// 选择新目录.
	connect(ui.pushButtonOpenDir, SIGNAL(clicked()), SLOT(OnBtnOpenDir()));	// 打开目录.


}

picsave::~picsave()
{

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
	int elapseMin = ui.lineEditPicDir->text().toInt();
	m_cfg.SetElapse(elapseMin * 60);

	// 开机自动运行.
	m_cfg;

	// 关闭.
	this->close();
}

void picsave::OnBtnCancel()
{
	this->close();
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