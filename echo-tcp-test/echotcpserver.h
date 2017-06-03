#ifndef ECHOTCP_H
#define ECHOTCP_H

#include <QObject>
#include <QTcpServer>

class EchoTCPServer : public QObject
{
    Q_OBJECT
public:
    explicit EchoTCPServer(QObject *parent = 0);

signals:
public:
    // 开始。 监听localPort， 将收到的数据原样返回
    void Start(int localPort);

    void Stop();

private slots:
    void OnNewConnection();
private:
    QTcpServer m_server;
};

#endif // ECHOTCP_H
