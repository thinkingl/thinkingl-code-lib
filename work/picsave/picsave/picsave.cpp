#include <windows.h>
#include <process.h>
#include "picsave.h"
#include <string>
#include <QDir>

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
}

void picsave::OnBtnCancel()
{
	this->close();
}

void picsave::OnBtnChooseNewDir()
{

}


// bool LocationFileInExplorer(IN CStringUtf8& filePath)
// {
// 	if (!IsFileExist(filePath))
// 	{
// 		assert(false && "文件不存在!");
// 		return false;
// 	}
// 
// 	StringAnsi cmd = "Explorer /e,/select, ";
// 	cmd += Utf8ToAnsi(filePath);
// 
// 	UINT ret = WinExec(cmd.c_str(), SW_SHOWNORMAL);
// 	LOGLOW() << cmd << "\tret:" << ret;
// 	return ret > 31;
// }

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