#ifndef PROXYCHANNELTRANSLATION_H
#define PROXYCHANNELTRANSLATION_H

#include <QObject>
#include <QTcpServer>

class ProxyChannelTranslation : public QObject
{
	Q_OBJECT

public:
	ProxyChannelTranslation(QObject *parent, QTcpSocket* localSock, QString remoteAddr, int remotePort );
	~ProxyChannelTranslation();

private slots:
	void OnLocalReadyRead();
	void OnLocalDisconnected();
	void OnLocalError(QAbstractSocket::SocketError);

	void OnRemoteReadyRead();
	void OnRemoteConnected();
	void OnRemoteDisconnected();
	void OnRemoteError(QAbstractSocket::SocketError);

private:
	QTcpSocket* m_localSock;
	QTcpSocket* m_remoteSock;
};

#endif // PROXYCHANNELTRANSLATION_H
