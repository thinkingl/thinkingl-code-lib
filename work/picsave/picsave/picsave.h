#ifndef PICSAVE_H
#define PICSAVE_H

#include <QtWidgets/QMainWindow>
#include "ui_picsave.h"
#include "picsaveconfig.h"
#include <QSystemTrayIcon>

class picsave : public QMainWindow
{
	Q_OBJECT

public:
	picsave(QWidget *parent = 0);
	~picsave();



public slots:
	// 确定按钮
	void OnBtnOk();

	// 取消按钮
	void OnBtnCancel();

	// 选择新的保存位置.
	void OnBtnChooseNewDir();
	// 打开图片保存目录.
	void OnBtnOpenDir();

	// 最小化.

	// 系统托盘处理.
	void iconIsActived(QSystemTrayIcon::ActivationReason reason);
	// 显示窗口.
	void OnTrayShow();
	// 退出程序.
	void OnTrayExit();

	void closeEvent(QCloseEvent *event);
private:
	// 读取配置,显示到界面上.
	void ReadConfig();

private:
	Ui::picsaveClass ui;

	// 程序配置.
	CPicSaveConfig m_cfg;
};

#endif // PICSAVE_H
