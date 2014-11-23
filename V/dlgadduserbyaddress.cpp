#include "dlgadduserbyaddress.h"
#include "VModel.h"
#include <QMessageBox>

CDlgAddUserByAddress::CDlgAddUserByAddress(CVModel* pVModel, QWidget *parent)
	: QDialog(parent)
	, m_pVModel( pVModel )
{
	ui.setupUi(this);

	connect( ui.okButton, SIGNAL( clicked() ), SLOT( OnButtonOk() ) );
}

CDlgAddUserByAddress::~CDlgAddUserByAddress()
{

}

void CDlgAddUserByAddress::OnButtonOk()
{
	QString strIP = ui.lineEditIP->text();
	if ( strIP.isEmpty() )
	{
		QMessageBox msgBox( QMessageBox::Critical, tr("Param error"), tr("IP can't be empty!") );
		msgBox.exec();
		return;
	}
	int port = ui.lineEditPort->text().toInt();
	if ( port <= 0 )
	{
		QMessageBox msgBox( QMessageBox::Critical, tr( "Param error" ), tr( "Port invalid!" ) );
		msgBox.exec();
		return;
	}

	Q_ASSERT( m_pVModel );
	if ( m_pVModel )
	{
		bool bOk = m_pVModel->AddUser( strIP.toUtf8().data(), port );
		if ( !bOk )
		{
			QMessageBox msgBox( QMessageBox::Critical, tr( "Error" ), tr( "Add user fail!" ) );
			msgBox.exec();
			return;
		}
	}

	accept();
}
