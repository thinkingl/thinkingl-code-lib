#ifndef PICSAVE_H
#define PICSAVE_H

#include <QtWidgets/QMainWindow>
#include "ui_picsave.h"

class picsave : public QMainWindow
{
	Q_OBJECT

public:
	picsave(QWidget *parent = 0);
	~picsave();

public slots:
	// 确定按钮

	// 取消按钮

	// 选择新的保存位置.

	// 设置新的.

	// 最小化.

private:
	Ui::picsaveClass ui;
};

#endif // PICSAVE_H
