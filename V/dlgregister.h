#ifndef DLGREGISTER_H
#define DLGREGISTER_H

#include <QDialog>
#include "ui_dlgregister.h"

class CDlgRegister : public QDialog
{
	Q_OBJECT

public:
	CDlgRegister(QWidget *parent = 0);
	~CDlgRegister();

private slots:
	// зЂВс.
	void OnButtonRegister();

private:

private:
	Ui::CDlgRegister ui;
};

#endif // DLGREGISTER_H
