#ifndef SOCKS5CLIENT_H
#define SOCKS5CLIENT_H

#include "idatatrans.h"
#include <QTcpSocket>

class TCPDest : public IDataTrans
{
    Q_OBJECT
public:
    explicit TCPDest(QObject *parent, QString remoteAddr, int remotePort);
    virtual ~TCPDest();

    // 处理数据.
    virtual bool TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
    virtual bool TransDataUp(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);

    // 向前关闭.
    virtual void CloseUp();
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
