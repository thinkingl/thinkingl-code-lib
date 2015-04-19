#ifndef CDLGLOGIN_H
#define CDLGLOGIN_H

#include <QDialog>
#include "ui_cdlglogin.h"

class CDlgLogin : public QDialog
{
	Q_OBJECT

public:
	CDlgLogin(QWidget *parent = 0);
	~CDlgLogin();

private slots:

	// зЂВс.
	void OnButtonRegister();

private:
	Ui::CDlgLogin ui;
};

#endif // CDLGLOGIN_H
