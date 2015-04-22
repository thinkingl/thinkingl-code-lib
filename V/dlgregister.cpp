#include "dlgregister.h"
#include <QMessageBox>
#include "UserConfig.h"

CDlgRegister::CDlgRegister(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.buttonRegister, SIGNAL(clicked()), SLOT(OnButtonRegister()));
}

CDlgRegister::~CDlgRegister()
{

}


void CDlgRegister::OnButtonRegister()
{
	QString email = ui.lineEditEmail->text();
	if (email.isEmpty())
	{
		QMessageBox::critical(this, tr("注册失败"), tr("Email不能为空!"), QMessageBox::Ok);
		ui.lineEditEmail->setFocus();
		return;
	}

	QString name = ui.lineEditName->text();
	if (name.isEmpty())
	{
		QMessageBox::critical(this, tr("注册失败"), tr("姓名不能为空"), QMessageBox::Ok);
		ui.lineEditName->setFocus();
		return;
	}

	QString password = ui.lineEditPassword->text();
	if (password.isEmpty())
	{
		QMessageBox::critical(this, tr("注册失败"), tr("密码不能为空"), QMessageBox::Ok);
		ui.lineEditPassword->setFocus();
		return;
	}

	// 进行注册.
	bool bOk = CUserConfig::RegisterUser(email, name, password);

	if (bOk)
	{
		// 关闭注册窗口.
		this->close();
	}
}