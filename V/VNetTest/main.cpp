
#include <QtCore/QCoreApplication>
#include "nettest.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	NetTest test(&a);

	return a.exec();
}
