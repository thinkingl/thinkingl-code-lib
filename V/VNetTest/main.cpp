
#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QObject>
#include <QTcpServer>


#include "nettest.h"


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	char c = '0';
	c += 128;
	c += 128;

	NetTest test(0);

	QTimer timer;

	QObject::connect(&timer, SIGNAL(timeout()), &test, SLOT(Proc()) );
	timer.start(1);

	QTcpServer svr;
	svr.listen(QHostAddress::Any,1090);

	return a.exec();
}
