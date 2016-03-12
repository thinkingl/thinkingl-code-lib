
#include <QtCore/QCoreApplication>
#include <QTimer>
#include <QObject>
#include <QTcpServer>


#include "nettest.h"

#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	char c = '0';
	c += 128;
	c += 128;



	QTcpServer svr;
	unsigned short port = 0;

	while ( port == 0 )
	{
		cout << "Input the port: ";
		QTextStream ts(stdin);

		ts >> port;

		if ( port == 0 )
		{
			cout << "Invalid port!" << endl;
		}
	}

	bool bOk = svr.listen(QHostAddress::Any, port);
	if (!bOk)
	{
		cout << "listen port " << port << " fail!" << endl;
	}
	else
	{
		cout << "listenning port " << port << endl;
	}

	NetTest test(0, &svr);

	QTimer timer;

	QObject::connect(&timer, SIGNAL(timeout()), &test, SLOT(Proc()));
	timer.start(1);

	return a.exec();
}
