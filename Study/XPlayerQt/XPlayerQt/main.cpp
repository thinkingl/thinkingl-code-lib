#include "xplayerqt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	XPlayerQt w;
	w.show();
	return a.exec();
}
