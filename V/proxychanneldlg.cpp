#include "proxychanneldlg.h"
#include <QLineEdit>

ProxyChannelDlg::ProxyChannelDlg(QWidget *parent)
	: QWidget(parent)
	, m_proxyChn(parent)
{
	ui.setupUi(this);

	connect(ui.pushButtonStart, SIGNAL(clicked()), SLOT(Start()));

}

ProxyChannelDlg::~ProxyChannelDlg()
{

}

void ProxyChannelDlg::Start()
{
	int localPort = ui.lineEditLocalPort->text().toUInt();
	QString remoteAddr = ui.lineEditRemoteAddr->text();
	int remotePort = ui.lineEditRemotePort->text().toUInt();

	m_proxyChn.Start(localPort, remoteAddr, remotePort);
}