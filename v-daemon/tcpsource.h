#ifndef TCPSOURCE_H
#define TCPSOURCE_H

#include <QObject>
#include <QTcpServer>
#include "idatatrans.h"


/**
*   TCP 数据接收.
*   +by lzx@2017-05-23
*/

class TCPSource : public IDataTrans
{
    Q_OBJECT

public:
    TCPSource( QObject* parent, QTcpSocket* localSock);
    virtual ~TCPSource();

    // 处理数据.
    virtual bool TransDataForward(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
    virtual bool TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);

    // 向前关闭.
    virtual void CloseBack();
private slots:
    void OnLocalReadyRead();
    void OnLocalDisconnected();
    void OnLocalError(QAbstractSocket::SocketError);
private:
    QTcpSocket* m_localSock;

};


#endif // TCPSOURCE_H
