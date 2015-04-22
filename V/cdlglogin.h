#ifndef CDLGLOGIN_H
#define CDLGLOGIN_H

#include <QDialog>
#include "ui_cdlglogin.h"

class CVModel;
class CDlgLogin : public QDialog
{
	Q_OBJECT

public:
	CDlgLogin( CVModel* pModel, QWidget *parent = 0);
	~CDlgLogin();

private slots:

	// 注册.
	void OnButtonRegister();

	// 登录.
	void OnButtonLogin();

private:
	// 刷新用户列表.
	void UpdateUserList();

private:
	CVModel* m_pModel;
private:
	Ui::CDlgLogin ui;
};

#endif // CDLGLOGIN_H
