#ifndef SOCKS5CLIENT_H
#define SOCKS5CLIENT_H

#include "idatatrans.h"
#include <QTcpSocket>

class TCPDest : public IDataTrans
{
    Q_OBJECT
public:
    explicit TCPDest(QObject *parent, QString remoteAddr, int remotePort);

    // 处理数据.
    virtual bool TransDataForward(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
    virtual bool TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
signals:

public slots:
    void OnSocks5ServerReadyRead();
    void OnSocks5ServerConnected();
    void OnSocks5ServerDisconnected();
    void OnSocks5ServerError(QAbstractSocket::SocketError);
private:
    QString m_socks5ServerAddr;
    quint16 m_socks5ServerPort;

    QTcpSocket* m_socks5TCPSocket;   // 连接Socks5代理服务器的socket.
};

#endif // SOCKS5CLIENT_H
