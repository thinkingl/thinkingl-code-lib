#include "cdlglogin.h"
#include <qmessagebox.h>
#include "UserConfig.h"

CDlgLogin::CDlgLogin(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.buttonRegister, SIGNAL(clicked()), SLOT(OnButtonRegister()));
}

CDlgLogin::~CDlgLogin()
{

}

void CDlgLogin::OnButtonRegister()
{
	ui.cmbEmail;
	ui.lineEditName;
	ui.lineEditPassword;

	QString email = ui.cmbEmail->currentText();
	if (email.isEmpty())
	{
		QMessageBox::critical(this, tr("注册失败"), tr("Email不能为空!"), QMessageBox::Ok);
		ui.cmbEmail->setFocus();
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
	bool bOk = CUserConfig::RegisterUser( email, name, password );

	// 用新用户登录.

}