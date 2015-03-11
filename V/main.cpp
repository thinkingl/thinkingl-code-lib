#include "vmainwin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CVMainWin w;
	w.show();
	return a.exec();
}
