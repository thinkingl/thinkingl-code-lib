#ifndef ECHOTCPSOCKET_H
#define ECHOTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class EchoTcpSocket: public QObject
{
    Q_OBJECT
public:
    EchoTcpSocket( QObject* parent, QTcpSocket* socket );

private:
    QTcpSocket* m_pSocket;

private slots:
    void OnLocalReadyRead();
    void OnLocalDisconnected();
    void OnLocalError(QAbstractSocket::SocketError);
};

#endif // ECHOTCPSOCKET_H
