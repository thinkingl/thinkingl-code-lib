#ifndef PROXYCHANNELTRANSLATION_H
#define PROXYCHANNELTRANSLATION_H

#include <QObject>
#include <QTcpServer>

class ProxyChannelTranslation : public QObject
{
	Q_OBJECT

public:
	ProxyChannelTranslation(QObject *parent, QTcpSocket* localSock, QTcpSocket* remoteSock );
	~ProxyChannelTranslation();

private slots:
	void OnReadyRead();
private:
	QTcpSocket* m_localSock;
	QTcpSocket* m_remoteSock;
};

#endif // PROXYCHANNELTRANSLATION_H
