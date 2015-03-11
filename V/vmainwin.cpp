#include "vmainwin.h"
#include "dlgadduserbyaddress.h"

CVMainWin::CVMainWin(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	// 连接菜单/工具栏的action.
	connect( ui.actionAddUserByAddress, SIGNAL(triggered()), this, SLOT( ConnectUserByAddress() ) );	// 连接一个用户.
}

CVMainWin::~CVMainWin()
{

}

void CVMainWin::ConnectUserByAddress()
{
	CDlgAddUserByAddress dlg( &m_vModel, this );
	if( QDialog::Accepted == dlg.exec() )
	{
		
	}
}