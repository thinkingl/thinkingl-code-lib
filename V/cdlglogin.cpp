#include "cdlglogin.h"
#include <qmessagebox.h>

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

	if (ui.cmbEmail->currentText().isEmpty())
	{
		QMessageBox::critical(this, tr("注册失败"), tr("Email不能为空!"), QMessageBox::Ok);
		ui.cmbEmail->setFocus();
		return;
	}

	if (ui.lineEditName->text().isEmpty())
	{
		QMessageBox::critical(this, tr("注册失败"), tr("姓名不能为空"), QMessageBox::Ok);
		ui.lineEditName->setFocus();
		return;
	}

	if (ui.lineEditPassword->text().isEmpty())
	{
		QMessageBox::critical(this, tr("注册失败"), tr("密码不能为空"), QMessageBox::Ok);
		ui.lineEditPassword->setFocus();
		return;
	}

	// 进行注册.

}