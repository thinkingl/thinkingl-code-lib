#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QString>
#include <QUdpSocket>
#include "idatatrans.h"

class UDPClient : public IDataTrans
{
    Q_OBJECT
public:
    explicit UDPClient(QObject *parent, QString remoteAddr, int remotePort);

    // 处理数据.
    virtual bool TransDataForward(const QByteArray& /* dataIn */, QByteArrayList& /* dataOut */ );
    virtual bool TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOut);

signals:

public slots:
    void readPendingDatagrams();
private:
    QString m_remoteAddr;
    int m_remotePort;

    QUdpSocket* m_udpSocket;
};

#endif // UDPSENDER_H
