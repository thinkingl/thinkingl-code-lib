#ifndef V_H
#define V_H

#include <QMainWindow>
#include "ui_vmainwin.h"
#include "VModel.h"

class CVMainWin : public QMainWindow
{
	Q_OBJECT

public:
	CVMainWin(CVModel* pModel, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~CVMainWin();

private slots: // 工具栏/菜单 处理.
		// 连接一个用户.
		void ConnectUserByAddress();

		// 登陆.
		void Login();

		void closeEvent(QCloseEvent * event);
private:
	Ui::vClass ui;

private:
	CVModel* m_pModel;
};

#endif // V_H
