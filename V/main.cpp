#include "vmainwin.h"
#include <QApplication>

#include "UserConfig.h"
#include "cdlglogin.h"
#include <qdialog.h>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

	CUserConfig::Init();

	QApplication a(argc, argv);

	QTextCodec* locale = QTextCodec::codecForLocale();
	QString kkk = locale->name();
	//QTextCodec::setCodecForLocale(locale);
	//QTextCodec::(QTextCodec::codecForLocale());
	

	// 弹出登录窗口.
	CDlgLogin dlgLogin;
	int ret = dlgLogin.exec();
	if (ret == QDialog::Accepted)
	{
		// 处理登录.

		CVMainWin w;
		w.show();
		ret = a.exec();
	}
	else
	{
		// 退出程序.
	}
//	CUserConfig::DestroyInstance();
	return ret;
}
