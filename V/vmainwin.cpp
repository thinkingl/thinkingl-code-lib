#include "vmainwin.h"
#include "dlgadduserbyaddress.h"
#include "cdlglogin.h"
#include <qevent.h>
#include "proxychanneldlg.h"

CVMainWin::CVMainWin(CVModel* pModel, QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
	, m_pModel( pModel )
	, m_pProxyChnDlg( 0 )
{
	ui.setupUi(this);

	// 连接菜单/工具栏的action.
	connect( ui.actionAddUserByAddress, SIGNAL(triggered()), this, SLOT( ConnectUserByAddress() ) );	// 连接一个用户.

	connect(ui.actionConfig, SIGNAL(triggered()), this, SLOT(ConfigProxyChn()));
}

CVMainWin::~CVMainWin()
{

}

void CVMainWin::ConnectUserByAddress()
{
	CDlgAddUserByAddress dlg( m_pModel, this );
	if( QDialog::Accepted == dlg.exec() )
	{
		
	}
}

void CVMainWin::closeEvent(QCloseEvent* event)
{
	event->accept();

	//this->quit();
}

void CVMainWin::Login()
{
	
}

void CVMainWin::ConfigProxyChn()
{
	if (m_pProxyChnDlg == 0)
	{
		m_pProxyChnDlg = new ProxyChannelDlg(this);
	}
	m_pProxyChnDlg->show();
}
