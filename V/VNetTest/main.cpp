
#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QObject>
#include "nettest.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	NetTest test(0);

	QTimer timer;

	QObject::connect(&timer, SIGNAL(timeout()), &test, SLOT(Proc()) );
	timer.start(1);

	return a.exec();
}
