#include "picsave.h"

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
	m_cfg;
}

void picsave::OnBtnCancel()
{
	this->close();
}

void picsave::OnBtnChooseNewDir()
{

}

void picsave::OnBtnOpenDir()
{

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
	ui.lineEditElapse->setText(QString::number(elapse));

	// 开机自动运行.

}