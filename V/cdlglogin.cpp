#include "cdlglogin.h"
#include <qmessagebox.h>
#include "UserConfig.h"
#include <QLineEdit>
#include "dlgregister.h"
#include "VModel.h"

CDlgLogin::CDlgLogin(CVModel* pModel, QWidget *parent)
	: QDialog(parent)
	, m_pModel( pModel )
{
	ui.setupUi(this);
	connect(ui.buttonLogin, SIGNAL(clicked()), SLOT(OnButtonLogin()));

	// 读取所有用户.
	QStringList allUser = CUserConfig::GetAllUserId();
	for (int i = 0; i < allUser.size();++i)
	{
//		CUserConfig* pCfg = CUserConfig::GetUserConfig(allUser[i]);
		CUserConfig cfg(allUser[i]);
		QString email = cfg.GetUserEmail();
		ui.cmbEmail->addItem(email);
	}
}

CDlgLogin::~CDlgLogin()
{

}

void CDlgLogin::OnButtonRegister()
{
	CDlgRegister dlgReg( this );
	dlgReg.exec();
}

void CDlgLogin::OnButtonLogin()
{
	QString email = ui.cmbEmail->currentText();
	if (email.isEmpty())
	{
		QMessageBox::critical(this, tr("登录失败"), tr("Email不能为空!"), QMessageBox::Ok);
		ui.cmbEmail->setFocus();
		return;
	}

	QString password = ui.lineEditPassword->text();
	if (password.isEmpty())
	{
		QMessageBox::critical(this, tr("登录失败"), tr("密码不能为空"), QMessageBox::Ok);
		ui.lineEditPassword->setFocus();
		return;
	}

	bool bOk = m_pModel->Login(email, password);
	if ( !bOk )
	{
		QMessageBox::warning(this, "登录失败", "登录失败,用户名或密码错误!", QMessageBox::Ok);
	}
	else
	{
		this->accept();
	}
}
