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
		QMessageBox::critical(this, tr("×¢²áÊ§°Ü"), tr("Email²»ÄÜÎª¿Õ!"), QMessageBox::Ok);
		ui.lineEditEmail->setFocus();
		return;
	}

	QString name = ui.lineEditName->text();
	if (name.isEmpty())
	{
		QMessageBox::critical(this, tr("×¢²áÊ§°Ü"), tr("ÐÕÃû²»ÄÜÎª¿Õ"), QMessageBox::Ok);
		ui.lineEditName->setFocus();
		return;
	}

	QString password = ui.lineEditPassword->text();
	if (password.isEmpty())
	{
		QMessageBox::critical(this, tr("×¢²áÊ§°Ü"), tr("ÃÜÂë²»ÄÜÎª¿Õ"), QMessageBox::Ok);
		ui.lineEditPassword->setFocus();
		return;
	}

	// ½øÐÐ×¢²á.
	bool bOk = CUserConfig::RegisterUser(email, name, password);

	if (bOk)
	{
		// ¹Ø±Õ×¢²á´°¿Ú.
		this->close();
	}
}