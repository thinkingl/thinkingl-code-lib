#include "vmainwin.h"
#include <QApplication>

#include "UserConfig.h"
#include "cdlglogin.h"
#include <qdialog.h>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
	//QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

	qApp->addLibraryPath("C:\\01-code\\qt\\Qt5.4.1-x86-vc2013\\5.4\\msvc2013\\plugins");

	QApplication a(argc, argv);


	CUserConfig::Init();

	CVModel vModel;


	QTextCodec* locale = QTextCodec::codecForLocale();
	QString kkk = locale->name();
	//QTextCodec::setCodecForLocale(locale);
	//QTextCodec::(QTextCodec::codecForLocale());
	

	// 弹出登录窗口.
	CDlgLogin dlgLogin(&vModel);
	int ret = dlgLogin.exec();
	if (ret == QDialog::Accepted)
	{
		// 处理登录.

		CVMainWin w(&vModel);
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
