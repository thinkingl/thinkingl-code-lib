#ifndef PROXYCHANNELDLG_H
#define PROXYCHANNELDLG_H

#include <QWidget>
#include "ui_proxychanneldlg.h"
#include "proxychannel.h"

class ProxyChannelDlg : public QWidget
{
	Q_OBJECT

public:
	ProxyChannelDlg(QWidget *parent = 0);
	~ProxyChannelDlg();

private slots:
	void Start();
private:
	Ui::ProxyChannelDlg ui;

	ProxyChannel m_proxyChn;
};

#endif // PROXYCHANNELDLG_H
