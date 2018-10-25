#ifndef ETHPROXYCHANNEL_H
#define ETHPROXYCHANNEL_H

/**
  * ETH Proxy的一个通道.
  * 接收局域网里ETH挖矿程序的连接和数据, 转发给矿池.
  * 接收矿池的数据,转发给局域网的挖矿程序, 局域网挖矿程序以为自己连接的就是矿池.
  * 挖矿程序和矿池任意一端断链, 断开连接, 销毁这个通道.
  * +by lzx@2017-07-11
*/
#include <QObject>
#include <QTcpSocket>

class ETHProxyChannel : public QObject
{
    Q_OBJECT
public:
    explicit ETHProxyChannel(QObject *parent, QTcpSocket* localSock, QString remoteAddr, int remotePort);

public:
    ~ETHProxyChannel();

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

signals:

public slots:
};

#endif // ETHPROXYCHANNEL_H
