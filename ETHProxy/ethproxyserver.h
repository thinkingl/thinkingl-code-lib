#ifndef ETHPROXYSERVER_H
#define ETHPROXYSERVER_H

/**
  * ETH Proxy的服务类
  * 在端口上监听,等待挖矿程序的连接
  * 为每个挖矿程序连接创建通信通道.
  * +by lzx@2017-07-11
*/

#include <QObject>
#include <QTcpServer>

class ETHProxyServer : public QObject
{
    Q_OBJECT
public:
    explicit ETHProxyServer(QObject *parent = 0);

    ~ETHProxyServer();

    // 开始。 监听localPort
    void Start(int localPort, const QString& remoteAddr, int remotePort);

    void Stop();

private slots:
    void OnNewConnection();
private:
    QTcpServer m_server;

    QString m_remoteAddr;
    int m_remotePort;
signals:

public slots:
};

#endif // ETHPROXYSERVER_H
