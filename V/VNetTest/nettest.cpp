#include "nettest.h"
#include <QTextStream>
#include <QThread>

NetTest::NetTest(QObject *parent)
	: QObject(parent)
{
	QThread* myThread = new QThread(this);
	myThread->start();
	this->moveToThread(myThread);
}

NetTest::~NetTest()
{

}

void NetTest::Proc()
{
	QTextStream qout(stdout);
	QTextStream qin(stdin);

	QString kk;
	qin >> kk;
	qout << kk << endl;
}
