#ifndef CDLGADDUSERBYADDRESS_H
#define CDLGADDUSERBYADDRESS_H

#include <QDialog>
#include "ui_dlgadduserbyaddress.h"

class CDlgAddUserByAddress : public QDialog
{
	Q_OBJECT

public:
	CDlgAddUserByAddress(QWidget *parent = 0);
	~CDlgAddUserByAddress();

private:
	Ui::CDlgAddUserByAddress ui;
};

#endif // CDLGADDUSERBYADDRESS_H
