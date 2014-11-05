#ifndef V_H
#define V_H

#include <QtGui/QMainWindow>
#include "ui_vmainwin.h"

class CVMainWin : public QMainWindow
{
	Q_OBJECT

public:
	CVMainWin(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CVMainWin();

private:
	Ui::vClass ui;
};

#endif // V_H
