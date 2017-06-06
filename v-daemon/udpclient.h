#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QObject>
#include <QString>
#include <QUdpSocket>
#include <QTimer>
#include "idatatrans.h"

class UDPClient : public IDataTrans
{
    Q_OBJECT
public:
    explicit UDPClient(QObject *parent, QHostAddress serverAddr, int remotePort);
    virtual ~UDPClient();

    // 处理数据.
    virtual bool TransDataDown(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);
    virtual bool TransDataUp(const QByteArray& dataIn, QByteArrayList& dataOutForward, QByteArrayList& dataOutBack);

signals:

public slots:
    void readPendingDatagrams();

    void checkTimeout();
private:
    QHostAddress m_remoteAddr;
    int m_remotePort;

    QUdpSocket* m_udpSocket;

    QTimer m_timerCheckTimeout;
    time_t m_lastActiveTime;
};

#endif // UDPSENDER_H
