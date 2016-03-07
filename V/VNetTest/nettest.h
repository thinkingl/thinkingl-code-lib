#ifndef NETTEST_H
#define NETTEST_H

#include <QObject>

class NetTest : public QObject
{
	Q_OBJECT

public:
	NetTest(QObject *parent);
    ~NetTest();

public slots:
	void Proc();
private:
    
};

#endif // NETTEST_H
