#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "idatatrans.h"
#include <QUdpSocket>
#include <QMap>
#include <QTimer>

class UDPServer : public IDataTrans
{
    Q_OBJECT
public:
    explicit UDPServer(QObject *parent, int localPort, QString socks5ServerAddr, quint16 socks5ServerPort);

    // 处理数据.
    virtual bool TransDataForward(const QByteArray& /* dataIn */, QByteArrayList& /* dataOut */ );
    virtual bool TransDataBack(const QByteArray& dataIn, QByteArrayList& dataOut);

    // 回送数据.
    virtual bool InputDataBack(IDataTrans* pNextItem, const QByteArray& data);

signals:

private slots:
    void readPendingDatagrams();
    void CheckTableTimeout();
private:

    IDataTrans* CreateDataTrans( );
private:
    QUdpSocket* m_udpSocket;

    // UDP源地址同处理对象的双向映射.
    class UDPSenderAndDataTransTableItem
    {
    public:
        QHostAddress senderAddr;    // 发送者的地址.
        quint16 senderPort;         // 发送者的端口.
        IDataTrans* dataTrans;      // 数据处理对象.
        time_t lastActiveTime;      // 最后的活动时间.
    };
    typedef QList< UDPSenderAndDataTransTableItem > UDPSenderAndDataTransTable;
    UDPSenderAndDataTransTable m_udpSenderAndDataTransTable;

    // 检测超时的定时器.
    QTimer m_checkTimer;

    QString m_socks5ServerAddr;
    quint16 m_socks5ServerPort;
private:
    UDPSenderAndDataTransTableItem* findItem( const QHostAddress& senderAddr, quint16 senderPort );
    UDPSenderAndDataTransTableItem* findItem( IDataTrans* pTrans );
};

#endif // UDPSERVER_H
