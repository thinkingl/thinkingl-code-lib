#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "idatatrans.h"
#include <QUdpSocket>

class UDPServer : public IDataTrans
{
    Q_OBJECT
public:
    explicit UDPServer(QObject *parent, int localPort);

    // 处理数据.
    virtual bool TransDataForward(const QByteArray& /* dataIn */, QByteArrayList& /* dataOut */ );
    virtual bool TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOut);

signals:

public slots:
    void readPendingDatagrams();

private:
    QUdpSocket* m_udpSocket;
};

#endif // UDPSERVER_H
