#ifndef CDLGADDUSERBYADDRESS_H
#define CDLGADDUSERBYADDRESS_H

#include <QDialog>
#include "ui_dlgadduserbyaddress.h"

class CVModel;
class CDlgAddUserByAddress : public QDialog
{
	Q_OBJECT

public:
	CDlgAddUserByAddress(CVModel* pVModel, QWidget *parent = 0);
	~CDlgAddUserByAddress();

private:
	Ui::CDlgAddUserByAddress ui;

private Q_SLOTS:
	void OnButtonOk();

private:
	CVModel* m_pVModel;
};

#endif // CDLGADDUSERBYADDRESS_H
