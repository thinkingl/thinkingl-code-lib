#include "vmainwin.h"
#include <QApplication>

#include "UserConfig.h"

int main(int argc, char *argv[])
{
	CUserConfig::Init();

	QApplication a(argc, argv);
	CVMainWin w;
	w.show();
	return a.exec();
}
