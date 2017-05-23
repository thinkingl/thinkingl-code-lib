#ifndef PROXYCHANNEL_H
#define PROXYCHANNEL_H

#include <QObject>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

class ProxyChannel : public QObject
{
	Q_OBJECT

public:
	ProxyChannel(QObject *parent);
	~ProxyChannel();

	// 开始。 监听localPort， 将收到的数据加密/解密后发往remoteAddr:port
	void Start(int localPort, const QString& remoteAddr, int remotePort);

	void Stop();

private slots:
	void OnNewConnection();
private:
	QTcpServer m_server;

	typedef QSet<QTcpSocket*> QSocketSet;
	QSocketSet m_localTcpSocketTable;

	QString m_remoteAddr;
	int m_remotePort;
};

#endif // PROXYCHANNEL_H
