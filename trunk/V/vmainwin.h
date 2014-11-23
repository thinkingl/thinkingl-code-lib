#ifndef V_H
#define V_H

#include <QtGui/QMainWindow>
#include "ui_vmainwin.h"
#include "VModel.h"

class CVMainWin : public QMainWindow
{
	Q_OBJECT

public:
	CVMainWin(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CVMainWin();

private slots: // ������/�˵� ����.
		// ����һ���û�.
		void ConnectUserByAddress();

private:
	Ui::vClass ui;

private:
	CVModel m_vModel;
};

#endif // V_H
