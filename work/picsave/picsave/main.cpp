#include "picsave.h"
#include <QtWidgets/QApplication>

// Æ¤·ô.
void InstallSkin(QApplication* pApp)
{

}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	InstallSkin(&a);

	picsave w;
	w.show();
	return a.exec();
}
